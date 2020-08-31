module vga_unit_test (
    input  logic        CLOCK_50,
    input  logic [3:0]  KEY,

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

    // Alias 50MHz clock
    logic clk;
    assign clk = CLOCK_50;

    // Synchronized reset
    logic reset;
    always_ff @ (posedge clk) begin
        reset <= ~(KEY[0]);  // the push buttons are active low
    end

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

    // TEST: ball module
    logic is_ball;
    ball ball (
        .keycode(8'b0),
        .*
    );

    always_comb begin: map_color
        if (is_ball) vga_data = 16'hffff;  // white
        else vga_data = 16'b0011100000001111;  // purple
    end: map_color
    

endmodule