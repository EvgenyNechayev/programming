
module d_trigger (
	input wire 	     rst_i,
	input wire 		  clk_i,
	input wire 		  en_i,
	input wire [7:0] data_i,
	
	output reg [7:0] data_o
);

	always @(posedge clk_i)
	begin
		if (rst_i)
		begin
			data_o <= 8'h00;
		end
		else if (en_i) 
		begin
			data_o <= data_i;
		end
	end
	
endmodule
