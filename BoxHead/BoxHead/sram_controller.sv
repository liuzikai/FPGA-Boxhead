module sram_controller (
    input  logic        sram_clk,    // 100 MHz clock
    input  logic        reset,       // Active-high reset signal

    input  logic        frame_clk,

    input  logic [9:0]  program_x,
                        program_y,
    input  logic [15:0] program_data,

    input  logic [15:0] background_data,

    input  logic [9:0]  vga_x,
                        vga_y,
    output logic [15:0] vga_data,

    output logic        SRAM_CE, 
                        SRAM_UB, 
                        SRAM_LB, 
                        SRAM_OE, 
                        SRAM_WE,
    output logic [19:0] SRAM_ADDR,
    inout  wire  [15:0] SRAM_DQ

);   

    // FF to help detect frame switch
    logic frame_clk_delayed, frame_clk_rising_edge;
    always_ff @ (posedge sram_clk) begin
        frame_clk_delayed <= frame_clk;
        frame_clk_rising_edge <= (frame_clk == 1'b1) && (frame_clk_delayed == 1'b0);
    end


    // Current frame index, alter between 0 and 1 by frame_clk
    logic display_frame;
    always_ff @ (posedge sram_clk) begin
        if (reset) display_frame <= 1'b0;
        else begin
            if (frame_clk_rising_edge) display_frame <= ~display_frame;
            else display_frame <= display_frame;
        end
    end

    // Current stage, increment circularly from 0 to 4 each SRAM cycle
    typedef enum logic [1:0] {  // 8-bit opcodes
        STAGE_WRITE_1 = 2'b00,  // program write #1
        STAGE_VGA     = 2'b01,  // VGA read 
        STAGE_WRITE_2 = 2'b10,  // program write #2
        STAGE_BG      = 2'b11   // background write
    } stage_e;
    stage_e stage;
    always_ff @ (posedge sram_clk) begin
        if (reset) stage <= STAGE_WRITE_1;
        else begin
            unique case (stage)
                STAGE_WRITE_1: stage <= STAGE_VGA;
                STAGE_VGA:     stage <= STAGE_WRITE_2;
                STAGE_WRITE_2: stage <= STAGE_BG;
                STAGE_BG:      stage <= STAGE_WRITE_1;
            endcase
        end
    end

    // VGA data register, update at VGA stage
    logic [15:0] vga_data_reg;
    always_ff @ (posedge sram_clk) begin
        if (reset) vga_data_reg <= 16'h0000;
        else begin
            if (stage == 2'b01) vga_data_reg <= SRAM_DQ;  // load the value at the beginning of third stage (stage has not changed yet)
            else vga_data_reg <= vga_data_reg;  // keep the value
        end
    end
    assign vga_data = vga_data_reg;
    
    /*
       Calculate offsets

       This module needs to run at 100MHz, so we try to avoid expensive calculations like * and even +.
       We have 20 bits for address, so we encode address as follows:

       {1 bit for alternating frame, 9 bits for Y, 10 bits for X}

       640: 1010000000 (10 bits)
       480: 111100000 (9 bits)
    */

    logic [19:0] program_addr, vga_addr;
    assign program_addr = {~display_frame, program_y[8:0], program_x};  // program write to hidden frame
    assign vga_addr = = {display_frame, vga_y[8:0], vga_x};  // vga write to display frame


    // Control SRAM based on state machine
    always_comb begin
        unique case (stage)
            STAGE_WRITE_1, STAGE_WRITE_2: begin  
                SRAM_ADDR = program_addr;
                SRAM_DQ = program_data;
                SRAM_WE = 1'b0;  // active low
                SRAM_OE = 1'b1;
            end
            STAGE_VGA: begin  // VGA read
                SRAM_ADDR = vga_addr;
                SRAM_DQ = {16{1'bZ}};
                SRAM_WE = 1'b1;
                SRAM_OE = 1'b0;  // active low
            end
            STAGE_BG: begin  // background write, to last read VGA pixel
                SRAM_ADDR = vga_addr;
                SRAM_DQ = background_data;
                SRAM_WE = 1'b0;  // active low
                SRAM_OE = 1'b1;
            end
          endcase
    end

    // These signals should always be active (low)
    assign SRAM_CE = 1'b0;
	assign SRAM_UB = 1'b0;
    assign SRAM_LB = 1'b0;

endmodule