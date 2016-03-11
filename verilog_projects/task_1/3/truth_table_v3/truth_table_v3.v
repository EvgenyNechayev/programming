
module truth_table_v3 (
	input wire [3:0] a_i,
	
	output wire 	  b_o
);

	assign b_o = (~a_i[3] & ~a_i[0]) | (a_i[3] & a_i[0]);
	
endmodule
