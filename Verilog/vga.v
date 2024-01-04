module vga (
	input CLOCK_50,
	input [9:0] SW,
	input KEY0,
	input KEY1,
	input GPIO1,
	output wire [9:0] LEDR,
	output wire VGA_HS,
	output wire VGA_VS,
	output reg [3:0] VGA_R,
	output reg [3:0] VGA_G,
	output reg [3:0] VGA_B,
	output wire [7:0] HEX0,
	output wire [7:0] HEX1,
	output wire [7:0] HEX2,
	output wire [7:0] HEX3,
	output wire [7:0] HEX4,
	output wire [7:0] HEX5
	);
	
	reg write_enable = 0;
	reg[16:0] write_address = 0;
	wire[16:0] read_address;

	reg [1:0] dataR = 0;
	reg [1:0] dataG = 0;
	reg [1:0] dataB = 0;

	wire [1:0] outR;
	wire [1:0] outG;
	wire [1:0] outB;

	wire dR;
	wire inDisplayArea;
	wire [9:0] displayX;
	wire [9:0] displayY;
	wire [7:0] rx_byte;
	wire KEY0_s;
	wire KEY1_s;
	wire write;
	wire reset;
	reg clk_25 = 0;
	wire [8:0] x1;
	wire [8:0] x2;
	wire [7:0] y1;
	wire [7:0] y2;
	wire [5:0] color;
	wire write_signal;
	wire GPIO1_s;
	// 3 bits * 3 * 320 * 240 = 86.4kB
	colorram ramR (.clock(CLOCK_50), .data(dataR), .rdaddress(read_address), .wraddress(write_address), .wren(write_enable), .q(readR));
	colorram ramG (.clock(CLOCK_50), .data(dataG), .rdaddress(read_address), .wraddress(write_address), .wren(write_enable), .q(readG));
	colorram ramB (.clock(CLOCK_50), .data(dataB), .rdaddress(read_address), .wraddress(write_address), .wren(write_enable), .q(readB));

	sync keyzero (.clk(CLOCK_50), .data(KEY0), .sdata(KEY0_s));
	sync keyone  (.clk(CLOCK_50), .data(KEY1), .sdata(KEY1_s));
	
	
	wire[9:0] SW_s;
	sync keyswitches (.clk(CLOCK_50), .data(SW[9:0]), .sdata(SW_s[9:0]));
	sync gpiosync (.clk(CLOCK_50), .data(GPIO1), .sdata(GPIO_s));
	defparam keyswitches.n = 10;	
	
	wire[2:0] byte_num;
	
	uart_rx recieve (.CLOCK_50(CLOCK_50), .dR(dR), .pinin(GPIO1), .oByte(rx_byte));
	hvsync_generator vga_sync (.clk(clk_25), .vga_h_sync(VGA_HS), .vga_v_sync(VGA_VS), .CounterX(displayX), .CounterY(displayY), .inDisplayArea(inDisplayArea));
	rect_decode decode (.CLOCK_50(CLOCK_50), .dR(dR), .data(rx_byte), .x1(x1), .x2(x2), .y1(y1), .y2(y2), .color(color), .write_signal(write_signal), .byte_num(byte_num));
	assign LEDR[4] = write_signal;
	assign LEDR[3] = dR;
	edge_detector resetdetector (.clk(CLOCK_50), .trigger(KEY0_s), .pulse_fall(reset));
	edge_detector writedetector (.clk(CLOCK_50), .trigger(KEY1_s), .pulse_fall(write));
	
	
	displayhex xvalue1 (.bits(0), .HEX(HEX5));
	displayhex xvalue2 (.bits(y1[7:4]), .HEX(HEX4));
	displayhex xvalue3 (.bits(y1[3:0]), .HEX(HEX3));
	displayhex xvalue4 (.bits(0), .HEX(HEX2));
	displayhex xvalue5 (.bits(y2[7:4]), .HEX(HEX1));
	displayhex xvalue6 (.bits(y2[3:0]), .HEX(HEX0));
//	displayhex writeaddress5(.bits(byte_num[2:0]), .HEX(HEX5));
//	displayhex writeaddress4(.bits(rx_byte[7:4]), .HEX(HEX4)); 
//	displayhex writeaddress3(.bits(rx_byte[3:0]), .HEX(HEX3));  
//	displayhex writeaddress2(.bits(count[11:8]), .HEX(HEX2)); 
//	displayhex writeaddress1(.bits(count[7:4]), .HEX(HEX1));
//	displayhex writeaddress0(.bits(count[3:0]), .HEX(HEX0));
	
	reg[12:0] count;
	always @(negedge dR) begin 
			count <= count + 1;
	end

//	assign x1 = SW[4:0]*4; //200;
//	assign x2 = SW[9:5]*4; //300;
//	assign y1 = SW[4:0]*4; 
//	assign y2 = SW[9:5]*4;
//	assign color = 6'h33;
	reg reset_state = 0; //(if 1, reseting)
	reg write_state = 1; //(if 1, writing)
	

	always @(posedge CLOCK_50) 
		begin
			clk_25 = clk_25+1;
			if (reset)
				begin 
					write_address <= 0;
					write_enable <= 1;
					dataR <= 0;
					dataG <= 0;
					dataB <= 0;
					reset_state <= 1;
				end
			else if (write_signal)
				begin
					write_address <= 0;
					write_enable <= 1;
					dataR <= 0;
					dataG <= 0;
					dataB <= 0;
					write_state <= 1;
				end
			else if (reset_state) 
				begin
					if (write_address == 75600)
						begin
							write_address <= 0;
							write_enable <= 0;
							reset_state <= 0;
						end
					else
						begin
							write_address <= write_address + 1;
							write_enable <= 1;
							reset_state <= 1;
						end
				end
			else if (write_state)
				begin
					if (write_address == 75600) 
						begin
							write_address <= 0;
							write_enable <= 0;
							write_state <= 0;
							dataR <= dataR;
							dataG <= dataG;
							dataB <= dataB;
						end
					else
						begin
							write_address <= write_address + 1;
							write_state <= 1;
							if (((write_address % 320) > x1)  && ((write_address % 320) < x2))
								begin
									if (((write_address / 320) > y1) && ((write_address / 320) < y2))
										begin
											write_enable <= 1;
											dataR <= color[5:4];
											dataG <= color[3:2]; 
											dataB <= color[1:0];
										end
								end
							else // Don't write, but don't reset either
								begin
									write_enable <= 0;
									dataR <= 0;
									dataG <= 0;	
									dataB <= 0;
								end
						end
				end
		end

	assign read_address = displayX/2 + (displayY/2)*320;
		always @(posedge clk_25)
			begin
				if (inDisplayArea) 
					begin
					 VGA_R[3:0] <= readR*4;
					 VGA_G[3:0] <= readG*4; //(displayX/2 > 200)*4; //readG;
					 VGA_B[3:0] <= readB*4; //(displayY/2 > 200)*4; //readB;
					 
					end
				else 
					begin
						VGA_R[3:0] <= 4'b0000;
						VGA_G[3:0] <= 4'b0000;
						VGA_B[3:0] <= 4'b0000;
					end
			end
endmodule