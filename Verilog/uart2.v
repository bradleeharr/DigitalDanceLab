module uart2 (
input CLOCK_50,
input [9:0] SW,
input [1:0] KEY,
inout wire [1:0] GPIO,
output reg [7:0] HEX4, HEX5, HEX0, HEX1,
output wire[7:0] HEX2, HEX3);

	wire dR;
	wire [7:0] Byte;
	reg transmit, lock;
	reg [31:0] lockTime;
	
	
	uart_rx recieve (.CLOCK_50(CLOCK_50), .dR(dR), .pinin(GPIO[1]), .oByte(Byte));
	uart_tx transmiter (.CLOCK_50(CLOCK_50), .SW(SW), .transmit(transmit), .pinout(GPIO[0]));
	always @(posedge CLOCK_50)
		begin 
			if(KEY[1] == 0 && lock == 0)
				begin
					transmit <= 1;
					lockTime <= 0;
				end
			if(transmit == 1)
				begin
					transmit <=0;
					lock <= 1;
				end
			if(lock ==1)
				begin
					lockTime<=lockTime +1;
					if(lockTime > 3000000)
						lock <= !KEY[1];
				end
		end
		
	reg[7:0] countdR;
	displayhex hex1 (.bits(countdR[7:4]), .HEX(HEX2));
	displayhex hex2 (.bits(countdR[3:0]), .HEX(HEX3));
	//UPDATE DATA DISPLAY WHEN DONE RECIEVING
	always @(negedge dR)
		begin
				countdR <= countdR + 1;
			case(Byte[7:4])
				4'b0000 : HEX5 = 8'b11000000;//0
				4'b0001 : HEX5 = 8'b11111001;//1
				4'b0010 : HEX5 = 8'b10100100;//2
				4'b0011 : HEX5 = 8'b10110000;//3
				4'b0100 : HEX5 = 8'b10011001;//4
				4'b0101 : HEX5 = 8'b10010010;//5
				4'b0110 : HEX5 = 8'b10000010;//6
				4'b0111 : HEX5 = 8'b11111000;//7
				4'b1000 : HEX5 = 8'b10000000;//8
				4'b1001 : HEX5 = 8'b10010000;//9
				4'b1010 : HEX5 = 8'b10001000;//A
				4'b1011 : HEX5 = 8'b10000011;//b
				4'b1100 : HEX5 = 8'b11000110;//c
				4'b1101 : HEX5 = 8'b10100001;//d
				4'b1110 : HEX5 = 8'b10000110;//E
				4'b1111 : HEX5 = 8'b10001110;//F
				default : HEX5 = 8'b11111111;//ALL ON
			endcase
			case(Byte[3:0])
				4'b0000 : HEX4 = 8'b11000000;//0
				4'b0001 : HEX4 = 8'b11111001;//1
				4'b0010 : HEX4 = 8'b10100100;//2
				4'b0011 : HEX4 = 8'b10110000;//3
				4'b0100 : HEX4 = 8'b10011001;//4
				4'b0101 : HEX4 = 8'b10010010;//5
				4'b0110 : HEX4 = 8'b10000010;//6
				4'b0111 : HEX4 = 8'b11111000;//7
				4'b1000 : HEX4 = 8'b10000000;//8
				4'b1001 : HEX4 = 8'b10010000;//9
				4'b1010 : HEX4 = 8'b10001000;//A
				4'b1011 : HEX4 = 8'b10000011;//b
				4'b1100 : HEX4 = 8'b11000110;//c
				4'b1101 : HEX4 = 8'b10100001;//d
				4'b1110 : HEX4 = 8'b10000110;//E
				4'b1111 : HEX4 = 8'b10001110;//F
				default : HEX4 = 8'b11111111;//ALL ON
			endcase
		end
always @(*)
		begin
			case(SW[7:4])
				4'b0000 : HEX1 = 8'b11000000;//0
				4'b0001 : HEX1 = 8'b11111001;//1
				4'b0010 : HEX1 = 8'b10100100;//2
				4'b0011 : HEX1 = 8'b10110000;//3
				4'b0100 : HEX1 = 8'b10011001;//4
				4'b0101 : HEX1 = 8'b10010010;//5
				4'b0110 : HEX1 = 8'b10000010;//6
				4'b0111 : HEX1 = 8'b11111000;//7
				4'b1000 : HEX1 = 8'b10000000;//8
				4'b1001 : HEX1 = 8'b10010000;//9
				4'b1010 : HEX1 = 8'b10001000;//A
				4'b1011 : HEX1 = 8'b10000011;//b
				4'b1100 : HEX1 = 8'b11000110;//c
				4'b1101 : HEX1 = 8'b10100001;//d
				4'b1110 : HEX1 = 8'b10000110;//E
				4'b1111 : HEX1 = 8'b10001110;//F
				default : HEX1 = 8'b11111111;//ALL ON
			endcase
			case(SW[3:0])
				4'b0000 : HEX0 = 8'b11000000;//0
				4'b0001 : HEX0 = 8'b11111001;//1
				4'b0010 : HEX0 = 8'b10100100;//2
				4'b0011 : HEX0 = 8'b10110000;//3
				4'b0100 : HEX0 = 8'b10011001;//4
				4'b0101 : HEX0 = 8'b10010010;//5
				4'b0110 : HEX0 = 8'b10000010;//6
				4'b0111 : HEX0 = 8'b11111000;//7
				4'b1000 : HEX0 = 8'b10000000;//8
				4'b1001 : HEX0 = 8'b10010000;//9
				4'b1010 : HEX0 = 8'b10001000;//A
				4'b1011 : HEX0 = 8'b10000011;//b
				4'b1100 : HEX0 = 8'b11000110;//c
				4'b1101 : HEX0 = 8'b10100001;//d
				4'b1110 : HEX0 = 8'b10000110;//E
				4'b1111 : HEX0 = 8'b10001110;//F
				default : HEX0 = 8'b11111111;//ALL ON
			endcase
		end
	

endmodule 

//UART transmit module
module uart_tx (
input CLOCK_50,
input [9:0] SW,
input transmit,
output reg pinout);

//STATE PARAMS
localparam IDLE 	= 2'b00;
localparam START 	= 2'b01;
localparam DATA	= 2'b10;
localparam STOP 	= 2'b11;
//reg pinout;

// 5208 clock_50 clocks per bit
localparam bit_period = 1250;

reg [7:0] TByte = 0;
reg [12:0] clockCount = 0;
reg [1:0] state 		= 0;
reg [2:0] idx			= 0;

always @(posedge CLOCK_50)
	begin
		case(state) 
			IDLE :
				begin
					pinout <= 1;
					idx <= 0;
					clockCount <= 0;
					
					if(transmit == 1'b1)
						begin
							TByte <= SW[7:0];
							state <= START;
						end
					else
						state <= IDLE;
				end
				
			START :
				begin
					pinout <= 1'b0;
					if(clockCount < bit_period)
						begin 
							clockCount <= clockCount + 1;
							state <= START;
						end
					else
						begin
							clockCount <= 0;
							state <= DATA;
						end
				end
				
			DATA : 
				begin
					pinout <= TByte[idx];
					if(clockCount < bit_period)
						begin
							clockCount <= clockCount + 1;
							state <= DATA;
						end
					else
						begin
							clockCount <= 0;
							
							if(idx < 7)
								begin
									idx <= idx + 1;
									state <= DATA;
								end
							else 
								begin
									idx <= 0;
									state <= STOP;
								end
						end
				end
		
			STOP :
				begin
					pinout <= 1;
					if(clockCount < bit_period)
						begin
							clockCount <= clockCount + 1;
							state <= STOP;
						end
					else 
						begin
							clockCount <= 0;
							state <= IDLE;
						end
				end
			default : state <= IDLE;
		endcase
	end



endmodule 

//UART revieve module
module uart_rx (
input CLOCK_50,
input pinin,
output reg dR,
output reg [7:0] oByte);

//state params
localparam IDLE 	= 2'b00;
localparam START 	= 2'b01;
localparam DATA	= 2'b10;
localparam STOP 	= 2'b11;

// 5208 clock_50 clocks per bit
localparam bit_period = 1250;

reg raw_Data 			= 1'b1;
reg Data					= 1'b1;

reg [12:0] clockCount = 0;
//reg [7:0] rByte  		= 0;
reg [1:0] state 		= 0;
reg [2:0] idx			= 0;

//meta stable
always @(posedge CLOCK_50)
    begin
      raw_Data <= pinin;
      Data   <= raw_Data;
    end

//STATE MACHINE
always @(posedge CLOCK_50)
	begin
	
		case(state)
			IDLE :
				begin
					dR 			<= 0;
					clockCount 	<=	0;
					idx			<=	0;
					
					if (Data == 1'b0)
						state <= START;
					else 
						state <= IDLE;
				end
				
			START :
				begin
					dR <= 1'b1;
					if (clockCount == bit_period / 2)
						begin
							// check middle if still start bit
							if (Data == 1'b0)
								begin
									clockCount 	<= 0;
									state			<= DATA;
								end
							// if not send back to idle state
							else
								state <= IDLE;
						end
					else
						// count until middle of period
						begin
							clockCount <= clockCount + 1;
							state <= START;
						end
				end
			
			DATA :
				begin
					dR <= 1'b1;
					//wait one period
					if(clockCount < bit_period)
						begin 
							clockCount <= clockCount + 1;
							state <= DATA;
						end
					else
						begin
						//assign data to current idx and reset clock
						clockCount <= 0;
						oByte[idx] <= Data;
						// increase idx until 7 then go to STOP
						if(idx < 7)
							begin
								idx <= idx + 1;
								state <= DATA;
							end
						else 
							begin
								idx <= 0;
								state <= STOP;
							end
						end
				end
				
			STOP :
				begin
					dR <= 1'b1;
					// wait one clock period
					if(clockCount < bit_period)
						begin 
							clockCount <= clockCount + 1;
							state <= STOP;
						end
					else 
						begin
							// reset and go back to idle
							clockCount <=0;
							dR <= 1'b0;	// fall edge data read
							state <= IDLE;
						end
				end
			default :
				state <= IDLE;
		endcase
		
	end
endmodule 

