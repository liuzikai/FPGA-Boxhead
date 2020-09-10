module boxhead_toplevel(
    input  logic        CLOCK_50,
    input  logic [3:0]  KEY,
    output logic [7:0]  LEDG,
    output logic [6:0]  HEX0, 
                        HEX1,

    // SRAM Interface
    output logic        SRAM_CE_N, 
                        SRAM_UB_N, 
                        SRAM_LB_N, 
                        SRAM_OE_N, 
                        SRAM_WE_N,
    output logic [19:0] SRAM_ADDR,
    inout  wire  [15:0] SRAM_DQ,

    // VGA Interface
    output logic [7:0]  VGA_R,        // VGA Red
                        VGA_G,        // VGA Green
                        VGA_B,        // VGA Blue
    output logic        VGA_CLK,      // VGA Clock
                        VGA_SYNC_N,   // VGA Sync signal
                        VGA_BLANK_N,  // VGA Blank signal
                        VGA_VS,       // VGA virtical sync signal
                        VGA_HS,       // VGA horizontal sync signal

    // CY7C67200 Interface
    inout  wire  [15:0] OTG_DATA,
    output logic [1:0]  OTG_ADDR,
    output logic        OTG_CS_N,
                        OTG_RD_N,
                        OTG_WR_N,
                        OTG_RST_N,
    input        [1:0]  OTG_INT,

    // SDRAM Interface
    output logic [12:0] DRAM_ADDR,
    output logic [1:0]  DRAM_BA,
    output logic        DRAM_CAS_N,
    output logic        DRAM_CKE,
    output logic        DRAM_CS_N,
    inout  logic [31:0] DRAM_DQ,
    output logic [3:0]  DRAM_DQM,
    output logic        DRAM_RAS_N,
    output logic        DRAM_WE_N,
    output logic        DRAM_CLK
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

    // ================================ Data Source ================================

    logic [9:0] program_x, program_y;
    logic [15:0] program_data;
    logic program_write;

    logic [17:0] src_addr;
    // logic [3:0] src_raw_data;
    logic [15:0] src_data;

    logic palette_index;

    logic current_frame;

    // on_chip_mem on_chip_mem (
    //     .clk(clk),
    //     .read_addr(src_addr),
    //     .data_out(src_raw_data)
    // );

    // zombie_palette zombie_palette (
    //     .index(src_raw_data),
    //     .color(src_data)
    // );

    on_chip_mem_core on_chip_mem_core (
        .clk(clk),
        .read_addr(src_addr),
        .data_out(src_data)
    );

    // ================================ SRAM Controller ================================

    // SRAM controller
    sram_controller sram_controller (
        .background_data(16'b1101111001111001),
        .*
    );

    // ================================ USB OTG ================================

    // Interface between NIOS II and EZ-OTG chip
    logic [1:0] hpi_addr;
    logic [15:0] hpi_data_in, hpi_data_out;
    logic hpi_r, hpi_w, hpi_cs, hpi_reset;
    hpi_io_intf hpi_io_inst (
        .Clk(clk),
        .Reset(reset),
        // Signals connected to NIOS II
        .from_sw_address(hpi_addr),
        .from_sw_data_in(hpi_data_in),
        .from_sw_data_out(hpi_data_out),
        .from_sw_r(hpi_r),
        .from_sw_w(hpi_w),
        .from_sw_cs(hpi_cs),
        .from_sw_reset(hpi_reset),
        // Signals connected to EZ-OTG chip
        .OTG_DATA(OTG_DATA),    
        .OTG_ADDR(OTG_ADDR),    
        .OTG_RD_N(OTG_RD_N),    
        .OTG_WR_N(OTG_WR_N),    
        .OTG_CS_N(OTG_CS_N),
        .OTG_RST_N(OTG_RST_N)
    );

    logic [7:0] keycode;

    hex_driver hex_driver_0 (keycode[3:0], HEX0);
    hex_driver hex_driver_1 (keycode[7:4], HEX1);

    // ================================ SOC (including Copy Engine) ================================

    logic copy_engine_execute, copy_engine_done;

    boxhead_soc boxhead_soc (
        .clk_clk(clk),         
        .reset_reset_n(1'b1),

        .sdram_wire_addr(DRAM_ADDR), 
        .sdram_wire_ba(DRAM_BA),   
        .sdram_wire_cas_n(DRAM_CAS_N),
        .sdram_wire_cke(DRAM_CKE),  
        .sdram_wire_cs_n(DRAM_CS_N), 
        .sdram_wire_dq(DRAM_DQ),   
        .sdram_wire_dqm(DRAM_DQM),  
        .sdram_wire_ras_n(DRAM_RAS_N),
        .sdram_wire_we_n(DRAM_WE_N), 
        .sdram_clk_clk(DRAM_CLK),

        .otg_hpi_address_export(hpi_addr),
        .otg_hpi_data_in_port(hpi_data_in),
        .otg_hpi_data_out_port(hpi_data_out),
        .otg_hpi_cs_export(hpi_cs),
        .otg_hpi_r_export(hpi_r),
        .otg_hpi_w_export(hpi_w),
        .otg_hpi_reset_export(hpi_reset),

        .keycode_export(keycode),

        .copy_engine_export_data_src_data(src_data),
        .copy_engine_export_data_src_addr(src_addr),
        .copy_engine_export_data_program_y(program_y),
        .copy_engine_export_data_program_x(program_x),
        .copy_engine_export_data_program_write(program_write),
        .copy_engine_export_data_program_data(program_data),
        .copy_engine_export_data_palette_index(palette_index),
        .copy_engine_export_data_current_frame(current_frame),

        .copy_engine_export_data_engine_done(copy_engine_done),
        .copy_engine_export_data_engine_execute(copy_engine_execute)
    );

    assign LEDG[0] = ~copy_engine_done;
    assign LEDG[1] = copy_engine_execute;
    assign LEDG[7] = current_frame;

endmodule