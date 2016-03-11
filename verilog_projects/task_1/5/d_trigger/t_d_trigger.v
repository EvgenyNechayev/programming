
module test_d_trigger;

	reg  	     rst_i;
	reg  		  clk_i;
	reg  		  en_i;
	reg  [7:0] data_i;
	
	wire [7:0] data_o;

	d_trigger d_trigger_instance (
	.rst_i(rst_i), .clk_i(clk_i), .en_i(en_i), .data_i(data_i), 
	.data_o(data_o)
	);
	
	always
		#10 clk_i = ~clk_i;
  
	initial
	begin
		rst_i = 1'b0;
		clk_i = 1'b0;
		en_i = 1'b0;
		data_i = 1'b0;

		#20;
		
		@(posedge clk_i)
		begin
			rst_i = 1'b1;
		end
		#25  rst_i = 1'b0;

		#50; 

		@(posedge clk_i) 
		begin
			data_i = 8'hFF;
			en_i = 1'b1;
		end
		
		#50;
		
		@(posedge clk_i) 
		begin
			rst_i = 1'b1;
		end
		#25  rst_i = 1'b0;
		
		#50;
		
		@(posedge clk_i) 
		begin
			data_i = 8'h77;
			en_i = 1'b0;
		end
		
		#50;
		
		@(posedge clk_i) 
		begin
			en_i = 1'b1;
		end
	end
	
	initial
	begin
		#400 $finish;
	end
	
endmodule
	