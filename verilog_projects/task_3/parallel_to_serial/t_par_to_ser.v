
module test_par_to_ser #(parameter PREAMBLE = 6);

	reg 		 rst_i;
	reg 		 clk_i;
	reg 		 en_i;
	reg [7:0] prl_data_i;
	
	wire		 ser_data_o;
	wire	 	 en_o;
	wire	 	 busy_o;
	
par_to_ser #(.PREAMBLE(PREAMBLE)) par_to_ser_instance
	(
		.rst_i(rst_i), .clk_i(clk_i), .en_i(en_i), .prl_data_i(prl_data_i),
		.ser_data_o(ser_data_o), .en_o(en_o), .busy_o(busy_o)
	);
	
	initial
   begin
		clk_i = 0;
		forever #10 clk_i = ~clk_i;
	end
		
  	initial
	begin
		rst_i = 1'b1;
		en_i = 1'b0;
		prl_data_i = 8'b0;
	
		@(negedge clk_i)
		begin
			rst_i = 1'b0;
			en_i = 1'b1;
			prl_data_i = 8'b10101001;
		end
		
		@(negedge clk_i)
		begin
			en_i = 1'b0;
		end

		@(negedge en_o) #40
		
		@(negedge clk_i)
		begin
			en_i = 1'b1;
			prl_data_i = 8'b10111010;
		end
		@(negedge clk_i)
		begin
			prl_data_i = 8'b10100010;
		end
		@(negedge clk_i)
		begin
			prl_data_i = 8'b11100011;
		end
		@(negedge clk_i)
		begin
			prl_data_i = 8'b10101010;
		end
		@(negedge clk_i)
		begin
			prl_data_i = 8'b10111111;
		end
		
		@(negedge clk_i)
		begin
			en_i = 1'b0;
		end	

		@(negedge en_o)
		begin
			#50 en_i = 1'b1;
			prl_data_i = 8'b10111001;
		end
		
		@(negedge clk_i);
		@(negedge clk_i)
		begin
			en_i = 1'b0;
		end		
		
		#120
		
		@(negedge clk_i)
		begin
			rst_i = 1'b1;
		end
		
		@(negedge clk_i)
		begin
			rst_i = 1'b0;
		end
	end
	
	initial
	begin
		#2000 $finish;
	end
	
endmodule
	