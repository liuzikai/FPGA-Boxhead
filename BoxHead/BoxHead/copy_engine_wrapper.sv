/************************************************************************
Avalon-MM Module for Copy Engine

Register Map:

 0: 10-bit dest_x_start
 1: 10-bit dest_x_end (exclusive)
 2: 10-bit dest_y_start
 3: 10-bit dest_y_end (exclusive)
 4: src_addr_start
 5: palette (character) index
 6: flip_x (0 or 1)
14: current_frame (0 or 1, read-only)
15: 1-bit execute/status
    Set to 1 to start execute
    Set to 0 to force terminate
    Will be set to 0 automatically after operation complete

************************************************************************/

module copy_engine_wrapper (
    input  logic        CLK,
    input  logic        RESET,
    
    // Avalon-MM Slave Signals
    input  logic        AVL_READ,
    input  logic        AVL_WRITE,
    input  logic        AVL_CS,
    input  logic [3:0]  AVL_BYTE_EN,
    input  logic [3:0]  AVL_ADDR,
    input  logic [31:0] AVL_WRITEDATA,
    output logic [31:0] AVL_READDATA,

    // Wires between data src
    output logic [19:0] src_addr,
    input  logic [15:0] src_data,

    // Wires between SRAM controller
    output logic [9:0]  program_x,
                        program_y,
    output logic [15:0] program_data,
    output logic        program_write,
    input  logic        current_frame,

    // Output to upper level
    output logic [1:0]  palette_index
);

    // Internal registers
    logic [31:0] regs[16];

    // Internal wires
    logic [31:0] reg_in[16];
    logic [9:0] dest_x_start, dest_x_end, dest_y_start, dest_y_end;
    logic [19:0] src_addr_start;
    logic flip_x;
    logic execute;
    logic done;

    copy_engine #(20) copy_engine (
        .clk(CLK),
        .reset(~RESET),
        .*
    );

    always_ff @ (posedge CLK) for (int i = 0; i < 16; ++i) regs[i] <= reg_in[i];

    always_comb begin
        for (int i = 0; i <= 6; i++)
        begin
            reg_in[i] = regs[i];  // pre-assign all bytes
            if (AVL_CS == 1'b1 && AVL_WRITE == 1'b1  && AVL_ADDR == i) begin
                // Replace result if byte enabled
                if (AVL_BYTE_EN[0] == 1'b1) reg_in[i][7:0] = AVL_WRITEDATA[7:0];
                if (AVL_BYTE_EN[1] == 1'b1) reg_in[i][15:8] = AVL_WRITEDATA[15:8];
                if (AVL_BYTE_EN[2] == 1'b1) reg_in[i][23:16] = AVL_WRITEDATA[23:16];
                if (AVL_BYTE_EN[3] == 1'b1) reg_in[i][31:24] = AVL_WRITEDATA[31:24];
            end
        end

        for (int i = 7; i <= 13; i++) reg_in[i] = 32'b0;

        reg_in[14] = {31'b0, current_frame};

        if (execute == 1'b1 && done == 1'b1) begin
            // Automatically set execute back to 0, which will also reset copy engine
            reg_in[15] = 32'h0000;
        end else if (AVL_CS == 1'b1 && AVL_WRITE == 1'b1  && AVL_ADDR == 15 && AVL_BYTE_EN[0] == 1'b1) begin
            // Accept Avalon input, including start and force stop
            reg_in[15] = {31'b0, AVL_WRITEDATA[0]};
        end else begin 
            reg_in[15] = regs[15];
        end

        if (AVL_CS == 1'b1 && AVL_READ == 1'b1) AVL_READDATA = regs[AVL_ADDR];
		else AVL_READDATA = 32'bX;
    end

    assign dest_x_start = regs[0][9:0];
    assign dest_x_end = regs[1][9:0];
    assign dest_y_start = regs[2][9:0];
    assign dest_y_end = regs[3][9:0];
    assign src_addr_start = regs[4][19:0];
    assign palette_index = regs[5][1:0];
    assign flip_x = regs[6][0];
    assign execute = regs[15][0];

endmodule
