
module boxhead_soc (
	clk_clk,
	copy_engine_export_data_src_data,
	copy_engine_export_data_src_addr,
	copy_engine_export_data_program_y,
	copy_engine_export_data_program_x,
	copy_engine_export_data_program_write,
	copy_engine_export_data_program_data,
	copy_engine_export_data_palette_index,
	copy_engine_export_data_current_frame,
	copy_engine_export_data_engine_done,
	copy_engine_export_data_engine_execute,
	key_wire_export,
	keycode_export,
	otg_hpi_address_export,
	otg_hpi_cs_export,
	otg_hpi_data_in_port,
	otg_hpi_data_out_port,
	otg_hpi_r_export,
	otg_hpi_reset_export,
	otg_hpi_w_export,
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
	sw_wire_export);	

	input		clk_clk;
	input	[15:0]	copy_engine_export_data_src_data;
	output	[19:0]	copy_engine_export_data_src_addr;
	output	[9:0]	copy_engine_export_data_program_y;
	output	[9:0]	copy_engine_export_data_program_x;
	output		copy_engine_export_data_program_write;
	output	[15:0]	copy_engine_export_data_program_data;
	output	[1:0]	copy_engine_export_data_palette_index;
	input		copy_engine_export_data_current_frame;
	output		copy_engine_export_data_engine_done;
	output		copy_engine_export_data_engine_execute;
	input	[3:0]	key_wire_export;
	output	[31:0]	keycode_export;
	output	[1:0]	otg_hpi_address_export;
	output		otg_hpi_cs_export;
	input	[15:0]	otg_hpi_data_in_port;
	output	[15:0]	otg_hpi_data_out_port;
	output		otg_hpi_r_export;
	output		otg_hpi_reset_export;
	output		otg_hpi_w_export;
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
	input	[15:0]	sw_wire_export;
endmodule
