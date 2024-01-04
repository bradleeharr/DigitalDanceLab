module dff_n (clk, D, Q);
	parameter n = 8;
	input clk;
	input wire [n-1:0] D;
	output reg [n-1:0] Q;
	
	always @(posedge clk) begin
		Q <= D;
	end
endmodule




module sync (clk, data, sdata);
	parameter n = 8;
	input wire clk;
	input wire [n-1:0] data;
	output wire [n-1:0] sdata;
	
	wire[n-1:0] data1;
	wire[n-1:0] data2;
	
	dff_n dff1 (.clk(clk), .D(data), .Q(data1));
	dff_n dff2 (.clk(clk), .D(data1), .Q(data2));
	dff_n dff3 (.clk(clk), .D(data2), .Q(sdata));
	defparam dff1.n = n;
	defparam dff2.n = n;
	defparam dff3.n = n;
endmodule





module edge_detector (clk, trigger, pulse_rise, pulse_fall);
	input wire clk;
	input wire trigger;
	output wire pulse_rise;
	output wire pulse_fall;
	
	wire Q1;
	wire Q2;
	dff_n dff1 (.clk(clk), .D(trigger), .Q(Q1));
	dff_n dff2 (.clk(clk), .D(Q1), .Q(Q2));
	defparam dff1.n = 1;
	defparam dff2.n = 1;
	
	assign pulse_rise =  Q1 & ~Q2;
	assign pulse_fall = ~Q1 &  Q2;
	
endmodule
	
	
	
	
	
module displayhex (input wire [3:0] bits, output reg [7:0] HEX); 
	always @(*) begin
		case (bits)
			4'h0 : HEX = ~8'b00111111;
			4'h1 : HEX = ~8'b00000110;
			4'h2 : HEX = ~8'b01011011;
			4'h3 : HEX = ~8'b01001111;
			4'h4 : HEX = ~8'b01100110;
			4'h5 : HEX = ~8'b01101101;
			4'h6 : HEX = ~8'b01111101;
			4'h7 : HEX = ~8'b00000111;
			4'h8 : HEX = ~8'b01111111;
			4'h9 : HEX = ~8'b01101111;
			4'hA : HEX = ~8'b01110111;
			4'hB : HEX = ~8'b01111100;
			4'hC : HEX = ~8'b00111001;
			4'hD : HEX = ~8'b01011110;
			4'hE : HEX = ~8'b01111001;
			4'hF : HEX = ~8'b01110001;
			default : HEX = ~8'b00000000;
		endcase
	end 
endmodule
