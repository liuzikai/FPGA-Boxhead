module copy_engine_unit_test (
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

    // ================================ Copy Engine ================================

    parameter SrcAddrWidth = 14;

    logic [9:0] program_x, program_y;
    logic [15:0] program_data;
    logic program_write;

    logic [SrcAddrWidth-1:0] src_addr;
    logic [15:0] src_data;

    logic copy_engine_execute;
    logic copy_engine_done;

    copy_engine #(SrcAddrWidth) copy_engine (
        .dest_x_start(470),
        .dest_x_end(570),
        .dest_y_start(290),
        .dest_y_end(390),
        .src_addr_start(0),
        .execute(copy_engine_execute),
        .done(copy_engine_done),
        .*
    );

    assign copy_engine_execute = ~frame_clk;  // trigger engine each frame

    assign LEDG[0] = ~copy_engine_done;
    assign LEDG[1] = copy_engine_execute;
    assign LEDG[7:2] = 6'b0;

    // ================================ Test Memory ================================

    snow_mem snow_mem (
        .clk(clk),
        .read_addr(src_addr),
        .data_out(src_data)
    );

    // ================================ SRAM ================================

    // SRAM controller
    sram_controller sram_controller (
        .background_data(16'b0000000000000000),
        .*
    );


endmodule