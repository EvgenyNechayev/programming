
module counter (
	input wire 		  rst_i,
	input wire 		  clk_i,
	input wire 		  en_i,
	
	output reg [3:0] data_o
	);
	
	always @(posedge clk_i)
	begin
		if (rst_i)
		begin
			data_o <= 4'b0000;
		end
		else if (en_i)
		begin
			data_o <= data_o + 1'b1;
		end
	end
	
endmodule
