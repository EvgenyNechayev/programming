
module test_coder
	#(
		parameter N = 1024,
					 M = 10
	);
	reg  [N-1:0] data_i;
	reg 		    enable_i;
	
	wire [M-1:0] data_o;

	coder coder_instance
		(.data_i(data_i), .enable_i(enable_i), .data_o(data_o));
	
	integer i;
	
	initial
	begin
		data_i = 1024'b1;
		enable_i = 1'b0;
		
		#5
		enable_i = 1'b1;
		
		for (i = 0; i < N; i = i + 1)
			begin
				#5	data_i = data_i << 1;
			end
	end
	
	initial
	begin
		#5150 $finish;
	end
	
endmodule
	