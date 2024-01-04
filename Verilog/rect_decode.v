module rect_decode
(
	input CLOCK_50,
	input wire [7:0] data,
	input dR,
	output reg [8:0] x1,
	output reg [8:0] x2,
	output reg [7:0] y1,
	output reg [7:0] y2,
	output reg [5:0] color,
	output reg write_signal,
	output reg[2:0] byte_num
);

// Update Decode Bits at negative edge of dR
initial byte_num = 0;
initial write_signal = 0;
always @(negedge dR) 
	begin
		if (data[7:0] == 8'hff)
			begin
				byte_num <= 0;
			end
		else
			begin
				byte_num <= (byte_num + 1);
			end
		case (byte_num)
			3'd0 : 
				begin
					x1 <= data[7:0]*2;
					x2 <= x2;
					y1 <= y1;
					y2 <= y2;
					color <= color;
					write_signal <= 0;
				end
			3'd1 :
				begin
					x1 <= x1;
					x2 <= data[7:0]*2;
					y1 <= y1;
					y2 <= y2;
					color <= color;
					write_signal <= 0;
				end
			3'd2 :
				begin
					x1 <= x1;
					x2 <= x2;
					y1 <= data[7:0];
					y2 <= y2;
					color <= color;
					write_signal <= 0;
				end
			3'd3 :
				begin
					x1 <= x1;
					x2 <= x2;
					y1 <= y1;
					y2 <= data[7:0];
					color <= color;
					write_signal <= 0;
				end
			3'd4 :
				begin
					x1 <= x1;
					x2 <= x2;
					y1 <= y1;
					y2 <= y2;
					color[5:0] <= data[5:0];
					write_signal <= 1;
				end
			default :
				begin
					color <= color;
					x1 <= x1;
					x2 <= x2;
					y1 <= y1;
					y2 <= y2;
					write_signal <= 0;
				end
		endcase
	end
endmodule