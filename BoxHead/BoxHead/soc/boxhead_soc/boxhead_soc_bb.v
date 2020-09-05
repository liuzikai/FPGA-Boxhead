
module boxhead_soc (
	clk_clk,
	reset_reset_n,
	sdram_clk_clk,
	sdram_wire_addr,
	sdram_wire_ba,
	sdram_wire_cas_n,
	sdram_wire_cke,
	sdram_wire_cs_n,
	sdram_wire_dq,
	sdram_wire_dqm,
	sdram_wire_ras_n,
	sdram_wire_we_n,
	copy_engine_export_src_data,
	copy_engine_export_src_addr,
	copy_engine_export_program_y,
	copy_engine_export_program_x,
	copy_engine_export_program_write,
	copy_engine_export_program_data,
	copy_engine_export_palette_index);	

	input		clk_clk;
	input		reset_reset_n;
	output		sdram_clk_clk;
	output	[12:0]	sdram_wire_addr;
	output	[1:0]	sdram_wire_ba;
	output		sdram_wire_cas_n;
	output		sdram_wire_cke;
	output		sdram_wire_cs_n;
	inout	[31:0]	sdram_wire_dq;
	output	[3:0]	sdram_wire_dqm;
	output		sdram_wire_ras_n;
	output		sdram_wire_we_n;
	input	[15:0]	copy_engine_export_src_data;
	output	[21:0]	copy_engine_export_src_addr;
	output	[9:0]	copy_engine_export_program_y;
	output	[9:0]	copy_engine_export_program_x;
	output		copy_engine_export_program_write;
	output	[15:0]	copy_engine_export_program_data;
	output	[1:0]	copy_engine_export_palette_index;
endmodule
