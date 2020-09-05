module boxhead_toplevel(
    input logic CLOCK_50
    input logic [15:0] SW,
    output logic [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7,
    output logic SRAM_CE, SRAM_UB, SRAM_LB, SRAM_OE, SRAM_WE,
    output logic [19:0] SRAM_ADDR,
    inout wire [15:0] SRAM_DQ
);

    parameter SrcAddrWidth = 22;

    on_chip_mem on_chip_mem (
        .clk(clk),
        .read_addr(src_addr),
        .data_out(src_raw_data)
    );

    zombie_palette zombie_palette (
        .index(src_raw_data),
        .color(src_data)
    );
    

endmodule