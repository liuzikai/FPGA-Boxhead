module sram_controller_unit_test (
    input  logic        CLOCK_50,

    output logic        SRAM_CE, SRAM_UB, SRAM_LB, SRAM_OE, SRAM_WE,
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
);

    logic vga_pll;


endmodule