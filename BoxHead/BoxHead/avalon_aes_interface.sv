/************************************************************************
Avalon-MM Interface for AES Decryption IP Core

Dong Kai Wang, Fall 2017

For use with ECE 385 Experiment 9
University of Illinois ECE Department

Register Map:

 0-3 : 4x 32bit AES Key
 4-7 : 4x 32bit AES Encrypted Message
 8-11: 4x 32bit AES Decrypted Message
   12: Not Used
	13: Not Used
   14: 32bit Start Register
   15: 32bit Done Register

************************************************************************/

/**
 * Module: avalon_aes_interface 
 * Input: CLK, RESET, AVL_READ, AVL_WRITE, AVL_CS, [3:0] AVL_BYTE_EN, 
          [3:0] AVL_ADDR, [31:0] AVL_WRITEDATA
 * Output: [31:0] AVL_READDATA, [31:0] EXPORT_DATA
 * Description: This is the hardware-software interface of AES decryptor.
                It's designed to be integrated with Avalon-MM.
 */

module avalon_aes_interface (
	// Avalon Clock Input
	input logic CLK,
	
	// Avalon Reset Input
	input logic RESET,
	
	// Avalon-MM Slave Signals
	input  logic AVL_READ,					// Avalon-MM Read
	input  logic AVL_WRITE,					// Avalon-MM Write
	input  logic AVL_CS,						// Avalon-MM Chip Select
	input  logic [3:0] AVL_BYTE_EN,		// Avalon-MM Byte Enable
	input  logic [3:0] AVL_ADDR,			// Avalon-MM Address
	input  logic [31:0] AVL_WRITEDATA,	// Avalon-MM Write Data
	output logic [31:0] AVL_READDATA,	// Avalon-MM Read Data
	
	// Exported Conduit
	output logic [31:0] EXPORT_DATA		// Exported Conduit Signal to LEDs
);

	// Internal registers
	logic [31:0] r[16];

	// Internal wires
	logic [31:0] next[16];
	logic AES_START, AES_DONE;
	logic [127:0] AES_KEY;
	logic [127:0] AES_MSG_ENC;
	logic [127:0] AES_MSG_DEC;

	AES aes(.*);

	always_comb
	begin

		// AES_KEY and AES_MSG_EN, writable from AVALON
		for (int i = 0; i <= 7; i++)
        begin
            if (RESET)
                next[i] = 32'h0;
			else if (AVL_CS == 1'b1 && AVL_WRITE == 1'b1  && AVL_ADDR == i)
			begin
				next[i] = r[i];  // pre-assign all bytes

				// Replace result if byte enabled
				if (AVL_BYTE_EN[0] == 1'b1) next[i][7:0] = AVL_WRITEDATA[7:0];
				if (AVL_BYTE_EN[1] == 1'b1) next[i][15:8] = AVL_WRITEDATA[15:8];
				if (AVL_BYTE_EN[2] == 1'b1) next[i][23:16] = AVL_WRITEDATA[23:16];
				if (AVL_BYTE_EN[3] == 1'b1) next[i][31:24] = AVL_WRITEDATA[31:24];
			end
            else
                next[i] = r[i];
		end
		AES_KEY = {r[3], r[2], r[1], r[0]};
		AES_MSG_ENC = {r[7], r[6], r[5], r[4]};

		// AES_MSG_DE, read only from AVALON
		next[8] = AES_MSG_DEC[31:0];
		next[9] = AES_MSG_DEC[63:32];
		next[10] = AES_MSG_DEC[95:64];
		next[11] = AES_MSG_DEC[127:96];

		// Empty
		next[12] = 32'h0;
		next[13] = 32'h0;

		// AES_START, writable from AVALON, only the last bit is stored
		if (RESET)
			next[14] = 32'h0;
		else if (AVL_CS == 1'b1 && AVL_WRITE == 1'b1  && AVL_ADDR == 14 && AVL_BYTE_EN[0] == 1'b1)
			next[14] = {r[14][31:1], AVL_WRITEDATA[0]};
		else
			next[14] = r[14];

		AES_START = r[14][0];
		
		// AES_DONE, read only from AVALON
		next[15] = {{31{1'b0}}, AES_DONE};

		// AVL_READDATA
		if (AVL_CS == 1'b1 && AVL_READ == 1'b1)
			AVL_READDATA = r[AVL_ADDR];
		else
			AVL_READDATA = 32'bX;
		
		// EXPORT_DATA
		EXPORT_DATA = {r[7][31:16], r[4][15:0]};
	end

	always_ff @ (posedge CLK)
	begin
		for (int i = 0; i < 16; i++)
        begin
            r[i] <= next[i];
        end
	end

endmodule
