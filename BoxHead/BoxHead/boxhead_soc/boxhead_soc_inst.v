	boxhead_soc u0 (
		.clk_clk                                (<connected-to-clk_clk>),                                //                     clk.clk
		.copy_engine_export_data_src_data       (<connected-to-copy_engine_export_data_src_data>),       // copy_engine_export_data.src_data
		.copy_engine_export_data_src_addr       (<connected-to-copy_engine_export_data_src_addr>),       //                        .src_addr
		.copy_engine_export_data_program_y      (<connected-to-copy_engine_export_data_program_y>),      //                        .program_y
		.copy_engine_export_data_program_x      (<connected-to-copy_engine_export_data_program_x>),      //                        .program_x
		.copy_engine_export_data_program_write  (<connected-to-copy_engine_export_data_program_write>),  //                        .program_write
		.copy_engine_export_data_program_data   (<connected-to-copy_engine_export_data_program_data>),   //                        .program_data
		.copy_engine_export_data_palette_index  (<connected-to-copy_engine_export_data_palette_index>),  //                        .palette_index
		.copy_engine_export_data_current_frame  (<connected-to-copy_engine_export_data_current_frame>),  //                        .current_frame
		.copy_engine_export_data_engine_done    (<connected-to-copy_engine_export_data_engine_done>),    //                        .engine_done
		.copy_engine_export_data_engine_execute (<connected-to-copy_engine_export_data_engine_execute>), //                        .engine_execute
		.key_wire_export                        (<connected-to-key_wire_export>),                        //                key_wire.export
		.keycode_export                         (<connected-to-keycode_export>),                         //                 keycode.export
		.otg_hpi_address_export                 (<connected-to-otg_hpi_address_export>),                 //         otg_hpi_address.export
		.otg_hpi_cs_export                      (<connected-to-otg_hpi_cs_export>),                      //              otg_hpi_cs.export
		.otg_hpi_data_in_port                   (<connected-to-otg_hpi_data_in_port>),                   //            otg_hpi_data.in_port
		.otg_hpi_data_out_port                  (<connected-to-otg_hpi_data_out_port>),                  //                        .out_port
		.otg_hpi_r_export                       (<connected-to-otg_hpi_r_export>),                       //               otg_hpi_r.export
		.otg_hpi_reset_export                   (<connected-to-otg_hpi_reset_export>),                   //           otg_hpi_reset.export
		.otg_hpi_w_export                       (<connected-to-otg_hpi_w_export>),                       //               otg_hpi_w.export
		.reset_reset_n                          (<connected-to-reset_reset_n>),                          //                   reset.reset_n
		.sdram_clk_clk                          (<connected-to-sdram_clk_clk>),                          //               sdram_clk.clk
		.sdram_wire_addr                        (<connected-to-sdram_wire_addr>),                        //              sdram_wire.addr
		.sdram_wire_ba                          (<connected-to-sdram_wire_ba>),                          //                        .ba
		.sdram_wire_cas_n                       (<connected-to-sdram_wire_cas_n>),                       //                        .cas_n
		.sdram_wire_cke                         (<connected-to-sdram_wire_cke>),                         //                        .cke
		.sdram_wire_cs_n                        (<connected-to-sdram_wire_cs_n>),                        //                        .cs_n
		.sdram_wire_dq                          (<connected-to-sdram_wire_dq>),                          //                        .dq
		.sdram_wire_dqm                         (<connected-to-sdram_wire_dqm>),                         //                        .dqm
		.sdram_wire_ras_n                       (<connected-to-sdram_wire_ras_n>),                       //                        .ras_n
		.sdram_wire_we_n                        (<connected-to-sdram_wire_we_n>),                        //                        .we_n
		.sw_wire_export                         (<connected-to-sw_wire_export>)                          //                 sw_wire.export
	);

