module sram_controller_unit_test (
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
	output logic [7:0]   LEDG
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
	 assign VGA_CLK = vga_clk;

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
	 logic sram_b_clk;
    sram_pll sram_pll (
        .inclk0(clk), 
        .c0(sram_clk),
		  .c1(sram_b_clk)
    );

    // TEST: fixed program coordinates and data
    logic [9:0] program_x, program_y;
    logic [15:0] program_data, background_data;
    assign program_x = 100;
    assign program_y = 200;
    assign program_data = 16'b0000000000000000;
    assign background_data = 16'b000000000011111;

    // SRAM controller
    sram_controller sram_controller (.*);


endmodule