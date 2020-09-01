module sram_controller_unit_test (
    input  logic        CLOCK_50,
    input  logic [3:0]  KEY,

    output logic        SRAM_CE, 
                        SRAM_UB, 
                        SRAM_LB, 
                        SRAM_OE, 
                        SRAM_WE,
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

    // ================================ Common ================================

    // Alias 50MHz clock
    logic clk;
    assign clk = CLOCK_50;

    // Synchronized reset
    logic reset;
    always_ff @ (posedge clk) begin
        reset <= ~(KEY[0]);  // the push buttons are active low
    end

    // ================================ VGA ================================

    // Setup VGA clock
    logic vga_clk;
    vga_pll vga_pll (
        .inclk0(clk), 
        .c0(vga_clk)
    );

    // Setup VGA controller
    logic [9:0] vga_x, vga_y;
    vga_controller vga_controller (.*);

    logic frame_clk;
    assign frame_clk = ~VGA_VS;

    // Setup color extend
    logic [15:0] vga_data;
    vga_color_extend vga_color_extend (.*);

    // ================================ SRAM ================================

    // Setup SRAM clock
    logic sram_clk;
    sram_pll sram_pll (
        .inclk0(clk), 
        .c0(sram_clk)
    );

    // TEST: fixed program coordinates and data
    logic [9:0] program_x, program_y;
    logic [15:0] program_data, background_data;
    assign program_x = 100;
    assign program_y = 200;
    assign program_data = 16'b11111000000000000;
    assign background_data = 16'h0000;

    // SRAM controller
    sram_controller sram_controller (.*);


endmodule