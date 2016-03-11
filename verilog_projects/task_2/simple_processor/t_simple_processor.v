
module test_simple_processor #(parameter BITS = 16);

	reg  [BITS-1:0] DIN;
	reg  		  		 Resetn;
	reg  		  		 Clock;
	reg  		  		 Run;
	
	wire  			 Done;
	wire [BITS-1:0] Bus;
	
simple_processor #(.BITS(BITS)) SP_instance
	(
		.DIN(DIN), .Resetn(Resetn), .Clock(Clock), .Run(Run),
		.Done(Done), .Bus(Bus)
	);
	
	always
		#10 Clock = ~Clock;
		
  	initial
	begin
		DIN = 0;
		Clock = 1'b1;
		Resetn = 1'b1;
		Run = 1'b0;
		
		@(negedge Clock)
		begin
			Resetn = 1'b0;
		end
		
		@(negedge Clock)	// 1 - load
		begin
			Run = 1'b1;
			DIN = 16'b001_001_000_0001111;
		end
		
		@(negedge Clock)
		begin
			Run = 1'b0;
		end

		@(negedge Clock)	// 2 - move
		begin
			Run = 1'b1;
			DIN = 16'b000_001_011_0000000;
		end
		
		@(negedge Clock)
		begin
			Run = 1'b0;
		end		

		@(negedge Clock)	// 3 - load
		begin
			Run = 1'b1;
			DIN = 16'b001_001_000_1111111;
		end
		
		@(negedge Clock)
		begin
			Run = 1'b0;
		end	

		@(negedge Clock)	// 4 - sub
		begin
			Run = 1'b1;
			DIN = 16'b011_001_011_0000000;
		end
		
		@(negedge Clock)
		begin
			Run = 1'b0;
		end	
	
		@(negedge Clock) ;
		@(negedge Clock) ;
		
		@(negedge Clock)	// 5 - add
		begin
			Run = 1'b1;
			DIN = 16'b010_001_011_0000000;
		end
		
		@(negedge Clock)
		begin
			Run = 1'b0;
		end			
	end
	
	initial
	begin
		#350 $finish;
	end
	
endmodule
	