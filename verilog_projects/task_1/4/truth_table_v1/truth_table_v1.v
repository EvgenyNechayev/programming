
module truth_table_v1 (
	input wire [1:0] a_i,
	
	output wire 	  b_o
);

	assign b_o = (~a_i[1] & a_i[0]) | (a_i[1] & a_i[0]);
	
endmodule
