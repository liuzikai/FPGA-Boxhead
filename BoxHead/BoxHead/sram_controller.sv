module sram_controller (
    input  logic        sram_clk,    // 100 MHz clock
    input  logic        sram_b_clk,  // 100 MHz clock with 25% duty cycle and -2.5ns offset
    input  logic        reset,       // Active-high reset signal

    input  logic        frame_clk,

    input  logic [9:0]  program_x,
                        program_y,
    input  logic [15:0] program_data,
    input  logic        program_write,

    input  logic [9:0]  vga_x,
                        vga_y,
    output logic [15:0] vga_data,

    input  logic [15:0] background_data,

    output logic        current_frame,
     
    input  logic        VGA_BLANK_N,

    output logic        SRAM_CE_N, 
                        SRAM_UB_N, 
                        SRAM_LB_N, 
                        SRAM_OE_N, 
                        SRAM_WE_N,
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
    assign current_frame = display_frame;

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
    assign vga_addr = {display_frame, vga_y[8:0], vga_x};  // vga write to display frame


    // Current stage, increment circularly from 0 to 4 each SRAM cycle
    typedef enum logic [1:0] {  // 8-bit opcodes
        STAGE_WRITE_1 = 2'b00,  // program write #1
        STAGE_VGA     = 2'b01,  // VGA read 
        STAGE_WRITE_2 = 2'b11,  // program write #2
        STAGE_BG      = 2'b10   // background write
    } stage_e;
    stage_e stage, stage_in;

    always_ff @ (posedge sram_clk) begin
        if (reset) stage <= STAGE_VGA;
        else stage <= stage_in;
    end

    always_comb begin
        unique case (stage)
            STAGE_BG:      stage_in = STAGE_WRITE_1;  
            STAGE_WRITE_1: stage_in = STAGE_VGA;  
            STAGE_VGA:     stage_in = STAGE_WRITE_2;  
            STAGE_WRITE_2: stage_in = STAGE_BG;  
        endcase
    end

    /*
        Control SRAM based on state machine

        We need synchronized output to control SRAM to avoid triggering operations with glitches.

        The least address setup and data hold time can be 0, so all can be performed at posedge.
    */
    logic [19:0] sram_addr_reg;
    logic [15:0] sram_out_reg;
    logic sram_write_enabled, sram_read_enabled;
    logic [15:0] vga_read_reg;

    always_ff @ (posedge sram_clk) begin
        // if (reset) begin
        //     sram_out_reg <= 16'b0;
        //     sram_write_enabled <= 0;
        //     sram_read_enabled <= 0;
        // end else begin
            unique case (stage)
                STAGE_BG: begin
                    // Enter Write #1 stage
                    sram_out_reg <= program_data;
                    sram_write_enabled <= program_write;
                    sram_read_enabled <= 0;
                end
                STAGE_WRITE_1: begin 
                    // Enter the VGA Read stage
                    sram_out_reg <= 16'b0000011111100000;  // this color should never shows
                    sram_write_enabled <= 0;
                    sram_read_enabled <= 1;
                end
                STAGE_VGA: begin
                    // Enter program write #2 stage
                    sram_out_reg <= program_data;
                    sram_write_enabled <= program_write;
                    sram_read_enabled <= 0;
                end
                STAGE_WRITE_2: begin
                    // Enter the background Write stage
                    sram_out_reg <= background_data;
                    sram_write_enabled <= VGA_BLANK_N;
                    sram_read_enabled <= 0;
                end
            endcase
        // end
    end

    // Launch SRAM addr in advance
    always_ff @ (posedge sram_b_clk) begin
        unique case (stage)
            STAGE_BG: begin
                // Ready for Program Write #1
                sram_addr_reg <= program_addr;
            end
            STAGE_WRITE_1: begin
                // Ready for VGA Read
                sram_addr_reg <= vga_addr;
            end
            STAGE_VGA: begin
                // Ready for Program Write #2
                sram_addr_reg <= program_addr;
            end
            STAGE_WRITE_2: begin
                // Ready for Background Write
                sram_addr_reg <= vga_addr;
            end
            
        endcase
    end

    // Fetch VGA Read data when proper
    always_ff @ (posedge sram_clk) begin
        if (stage == STAGE_VGA) vga_read_reg <= SRAM_DQ;
        else vga_read_reg <= vga_read_reg;
    end

    assign SRAM_CE_N = 1'b0;  // always active (low)
    assign SRAM_UB_N = 1'b0;  // always active (low)
    assign SRAM_LB_N = 1'b0;  // always active (low)

    assign SRAM_ADDR = sram_addr_reg;

    assign vga_data = vga_read_reg;
     
     always_comb begin
        SRAM_OE_N = ~sram_read_enabled;

        if (sram_write_enabled) begin
            SRAM_WE_N = sram_b_clk;
            SRAM_DQ = sram_out_reg;
        end else begin
            SRAM_WE_N = 1'b1;
            SRAM_DQ = {16{1'bZ}};
        end
     end

endmodule