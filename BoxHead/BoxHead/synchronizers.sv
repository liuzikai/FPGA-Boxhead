//These are synchronizers required for bringing asynchronous signals into the FPGA

// Synchronizer with no reset (for switches/buttons)
module sync (
	input  logic clk, d, 
	output logic q
);

	always_ff @ (posedge clk)
	begin
		q <= d;
	end

endmodule


// Synchronizer with reset to 0 (d_ff)
module sync_r0 (
	input  logic clk, Reset, d, 
	output logic q
);

	//initial
	//begin	
	//	q <= 1'b0;
	//end

	always_ff @ (posedge clk or posedge Reset)
	begin
		if (Reset)
			q <= 1'b0;
		else
			q <= d;
	end

endmodule

// Synchronizer with reset to 1 (d_ff)
module sync_r1 (
	input  logic clk, Reset, d, 
	output logic q
);

	//initial
	//begin	
	//	q <= 1'b1;
	//end

	always_ff @ (posedge clk or posedge Reset)
	begin
		if (Reset)
			q <= 1'b1;
		else
			q <= d;
	end

endmodule
