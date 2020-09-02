//------------------------------------------------------------------------------
// Company: 		 UIUC ECE Dept.
// Engineer:		 Stephen Kempf
//
// Create Date:    
// Design Name:    ECE 385 Lab 6 Given Code - Tristate buffer for SRAM
// Module Name:    tristate
//
// Comments:
//    Revised 02-13-2017
//    Spring 2017 Distribution
//
//------------------------------------------------------------------------------


module tristate #(N = 16) (
	input logic clk, 
	input logic write_enabled,
	input logic [N-1:0] data_write,  // data from Mem2IO
	output logic [N-1:0] data_read,  // data to Mem2IO
	inout wire [N-1:0] data          // inout bus to SRAM
);

// Registers are needed between synchronized circuit and asynchronized SRAM 
logic [N-1:0] data_read_buffer;

always_ff @(posedge clk)
begin
	// Always read data from the bus
	data_read_buffer <= data;
end

// Drive (write to) data bus only when write_enabled is active.
assign data = write_enabled ? data_write : {N{1'bZ}};

assign data_read = data_read_buffer;

endmodule
