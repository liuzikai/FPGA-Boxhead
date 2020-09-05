	boxhead_soc u0 (
		.clk_clk                          (<connected-to-clk_clk>),                          //                clk.clk
		.reset_reset_n                    (<connected-to-reset_reset_n>),                    //              reset.reset_n
		.sdram_clk_clk                    (<connected-to-sdram_clk_clk>),                    //          sdram_clk.clk
		.sdram_wire_addr                  (<connected-to-sdram_wire_addr>),                  //         sdram_wire.addr
		.sdram_wire_ba                    (<connected-to-sdram_wire_ba>),                    //                   .ba
		.sdram_wire_cas_n                 (<connected-to-sdram_wire_cas_n>),                 //                   .cas_n
		.sdram_wire_cke                   (<connected-to-sdram_wire_cke>),                   //                   .cke
		.sdram_wire_cs_n                  (<connected-to-sdram_wire_cs_n>),                  //                   .cs_n
		.sdram_wire_dq                    (<connected-to-sdram_wire_dq>),                    //                   .dq
		.sdram_wire_dqm                   (<connected-to-sdram_wire_dqm>),                   //                   .dqm
		.sdram_wire_ras_n                 (<connected-to-sdram_wire_ras_n>),                 //                   .ras_n
		.sdram_wire_we_n                  (<connected-to-sdram_wire_we_n>),                  //                   .we_n
		.copy_engine_export_src_data      (<connected-to-copy_engine_export_src_data>),      // copy_engine_export.src_data
		.copy_engine_export_src_addr      (<connected-to-copy_engine_export_src_addr>),      //                   .src_addr
		.copy_engine_export_program_y     (<connected-to-copy_engine_export_program_y>),     //                   .program_y
		.copy_engine_export_program_x     (<connected-to-copy_engine_export_program_x>),     //                   .program_x
		.copy_engine_export_program_write (<connected-to-copy_engine_export_program_write>), //                   .program_write
		.copy_engine_export_program_data  (<connected-to-copy_engine_export_program_data>),  //                   .program_data
		.copy_engine_export_palette_index (<connected-to-copy_engine_export_palette_index>)  //                   .palette_index
	);

