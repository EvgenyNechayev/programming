
module coder
	#(
		parameter N = 1024,
					 M = 10
	)
	(
		input wire [N-1:0] data_i,
		input wire 			 enable_i,
	
		output reg [M-1:0] data_o
	);
	
	integer i;
	
	always @*
	begin
		data_o = 10'b0;
		if (enable_i)
			for (i = 0; i < N; i = i + 1)
				if (data_i[i] == 1'b1)
					data_o = i[M-1:0];
	end
	
endmodule
