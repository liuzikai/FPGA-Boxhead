module sram_controller_motion_test (
    input  logic        CLOCK_50,
    input  logic [3:0]  KEY,

    output logic        SRAM_CE_N, 
                        SRAM_UB_N, 
                        SRAM_LB_N, 
                        SRAM_OE_N, 
                        SRAM_WE_N,
    output logic [19:0] SRAM_ADDR,
    inout  wire  [15:0] SRAM_DQ,

    output logic [7:0]  VGA_R,        // VGA Red
                        VGA_G,        // VGA Green
                        VGA_B,        // VGA Blue
    output logic        VGA_CLK,      // VGA Clock
                        VGA_SYNC_N,   // VGA Sync signal
                        VGA_BLANK_N,  // VGA Blank signal
                        VGA_VS,       // VGA virtical sync signal
                        VGA_HS,       // VGA horizontal sync signal
	output logic [7:0]  LEDG
);

    // ================================ Common ================================

    // Setup clock tree
    logic clk, sram_clk, sram_b_clk;
    assign clk = CLOCK_50;
    sram_pll sram_pll (
        .inclk0(clk), 
        .c0(sram_clk),
		.c1(sram_b_clk),
		.c2(VGA_CLK)
    );

    // Synchronized reset
    logic reset;
    always_ff @ (posedge clk) begin
        reset <= ~(KEY[0]);  // the push buttons are active low
    end

    // ================================ VGA ================================
	

    // Setup VGA controller
    logic [9:0] vga_x, vga_y;
    vga_controller vga_controller (.*);

    logic frame_clk;
    assign frame_clk = ~VGA_VS;

    // Setup color extend
    logic [15:0] vga_data;
    vga_color_extend vga_color_extend (.*);

    // ================================ SRAM ================================

    // TEST: ball module
     logic is_ball;
     ball ball (
         .keycode(8'b0),
         .*
     );

    // SRAM controller
    sram_controller sram_controller (
        .sram_clk(sram_clk),
        .sram_b_clk(sram_b_clk),
        .reset(reset),
        .frame_clk(frame_clk),
        .program_x(vga_x),
        .program_y(vga_y),
        .program_data(is_ball ? 16'b1111100000000000 : 16'b1111100000011111),
        .program_write(1'b1),
        .background_data(16'b0000000000011111),
        .vga_x(vga_x),
        .vga_y(vga_y),
        .vga_data(vga_data),
		.VGA_BLANK_N(VGA_BLANK_N),
        .SRAM_CE_N(SRAM_CE_N),
        .SRAM_UB_N(SRAM_UB_N),
        .SRAM_LB_N(SRAM_LB_N),
        .SRAM_OE_N(SRAM_OE_N),
        .SRAM_WE_N(SRAM_WE_N),
        .SRAM_ADDR(SRAM_ADDR),
        .SRAM_DQ(SRAM_DQ)
    );


endmodule