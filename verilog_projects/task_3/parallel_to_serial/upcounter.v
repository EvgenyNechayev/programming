
module upcounter
	(
	input  wire 	  rst_i,
	input  wire 	  clk_i,
	input  wire 	  en_i,
	
	output wire	[3:0] count_o
	);

	
	reg [3:0] count;
	
	assign count_o = count;
	
	always @(posedge clk_i)
	begin
		if (rst_i)
			count <= 4'b0;
		else if (en_i)
			count <= count + 1'b1;
	end
	
endmodule
