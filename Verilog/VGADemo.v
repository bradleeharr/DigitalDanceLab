SOURCE: https://ktln2.org/2018/01/23/implementing-vga-in-verilog/
module VGADemo(
    input CLOCK_50,
    output reg [2:0] pixel,
	 output reg [3:0] VGA_R,
	 output reg [3:0] VGA_G,
	 output reg [3:0] VGA_B,
	 output VGA_HS,
	 output VGA_VS
);
	reg clk_25 = 0;
	always @(posedge CLOCK_50) begin
		clk_25 = clk_25+1;
	end
	
   wire inDisplayArea;
   wire [9:0] CounterX;

   hvsync_generator hvsync(
      .clk(clk_25),
      .vga_h_sync(VGA_HS),
      .vga_v_sync(VGA_VS),
      .CounterX(CounterX),
      //.CounterY(CounterY),
      .inDisplayArea(inDisplayArea)
    );

    always @(posedge clk_25)
    begin
      if (inDisplayArea)
        {VGA_R[2:0], VGA_G[2:0], VGA_B[2:0]} <= CounterX[9:1];
      else // if it's not to display, go dark
        VGA_R <= 3'b000;
    end

endmodule
SOURCE: https://ktln2.org/2018/01/23/implementing-vga-in-verilog/
module hvsync_generator(
    input clk,
    output vga_h_sync,
    output vga_v_sync,
    output reg inDisplayArea,
    output reg [9:0] CounterX,
    output reg [9:0] CounterY
  );
    reg vga_HS, vga_VS;

    wire CounterXmaxed = (CounterX == 800); // 16 + 48 + 96 + 640
    wire CounterYmaxed = (CounterY == 525); // 10 + 2 + 33 + 480

    always @(posedge clk)
    if (CounterXmaxed)
      CounterX <= 0;
    else
      CounterX <= CounterX + 1;

    always @(posedge clk)
    begin
      if (CounterXmaxed)
      begin
        if(CounterYmaxed)
          CounterY <= 0;
        else
          CounterY <= CounterY + 1;
      end
    end

    always @(posedge clk)
    begin
      vga_HS <= (CounterX > (640 + 16) && (CounterX < (640 + 16 + 96)));   // active for 96 clocks
      vga_VS <= (CounterY > (480 + 10) && (CounterY < (480 + 10 + 2)));   // active for 2 clocks
    end

    always @(posedge clk)
    begin
        inDisplayArea <= (CounterX < 640) && (CounterY < 480);
    end

    assign vga_h_sync = ~vga_HS;
    assign vga_v_sync = ~vga_VS;

endmodule