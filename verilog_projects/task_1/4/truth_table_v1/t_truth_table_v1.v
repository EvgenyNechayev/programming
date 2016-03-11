
module test_truth_table_v1;

	reg [1:0] a_i;
	
	wire 		 b_o;
	
	truth_table_v1 truth_table_v1_instance (.a_i(a_i), .b_o(b_o));
	
	initial
	begin
		#20	a_i = 2'b00;
		#20	a_i = 2'b01;
		#20	a_i = 2'b10;
		#20	a_i = 2'b11;
	end
	
	initial
	begin
		#100 $finish;
	end
	
endmodule
	