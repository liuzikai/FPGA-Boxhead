
module sram_controller_single_toplevel (
    input  logic        CLOCK_50,

    output logic        SRAM_CE, 
                        SRAM_UB, 
                        SRAM_LB, 
                        SRAM_OE, 
                        SRAM_WE,
    output logic [19:0] SRAM_ADDR,
    inout  wire  [15:0] SRAM_DQ
);

    logic [15:0] vga_data;
    sram_controller sram_controller (
        .sram_clk(CLOCK_50),
        .reset(1'b0),
        .frame_clk(1'b0),
        .program_x(10'b0),
        .program_y(10'b0),
        .program_data(16'b0),
        .background_data(16'b0),
        .vga_x(10'b0),
        .vga_y(10'b0),
        .vga_data(16'b0),
        .*
    );


endmodule