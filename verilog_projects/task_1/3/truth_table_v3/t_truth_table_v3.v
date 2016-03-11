
module test_truth_table_v3;

	reg [3:0] a_i;
	
	wire 		 b_o;
	
	truth_table_v3 truth_table_v3_instance (.a_i(a_i), .b_o(b_o));
	
	integer i;
	
	initial
	begin
		for (i = 0; i < 16; i = i + 1)
			#5	a_i = i[3:0];
	end
	
	initial
	begin
		#100 $finish;
	end
	
endmodule
	