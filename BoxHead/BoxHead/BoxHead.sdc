## Generated SDC file "lab7.sdc"
## Revised by liuzikai on 2020.04.22

## Copyright (C) 1991-2014 Altera Corporation. All rights reserved.
## Your use of Altera Corporation's design tools, logic functions 
## and other software and tools, and its AMPP partner logic 
## functions, and any output files from any of the foregoing 
## (including device programming or simulation files), and any 
## associated documentation or information are expressly subject 
## to the terms and conditions of the Altera Program License 
## Subscription Agreement, the Altera Quartus II License Agreement,
## the Altera MegaCore Function License Agreement, or other 
## applicable license agreement, including, without limitation, 
## that your use is for the sole purpose of programming logic 
## devices manufactured by Altera and sold by Altera or its 
## authorized distributors.  Please refer to the applicable 
## agreement for further details.


## VENDOR  "Altera"
## PROGRAM "Quartus II"
## VERSION "Version 14.1.0 Build 186 12/03/2014 SJ Web Edition"

## DATE    "Sun Feb 15 03:20:54 2015"

##
## DEVICE  "EP4CE115F29C8"
##


#**************************************************************
# Time Information
#**************************************************************

set_time_format -unit ns -decimal_places 2



#**************************************************************
# Create Clock
#**************************************************************

# create_clock -name {altera_reserved_tck} -period 100.00 -waveform { 0.00 50.00 } [get_ports {altera_reserved_tck}]
create_clock -name {main_clk_50} -period 20.00 -waveform { 0.00 10.00 } [get_ports {CLOCK_50}]


#**************************************************************
# Create Generated Clock
#**************************************************************

create_generated_clock -name {vga_clk_25} -source [get_ports {CLOCK_50}] -duty_cycle 50.00 -multiply_by 1 -divide_by 2 -phase 0.00 -master_clock {main_clk_50} [get_pins {vga_pll|altpll_component|auto_generated|pll1|clk[0]}]
create_generated_clock -name {sram_clk_100_50} -source [get_ports {CLOCK_50}] -duty_cycle 50.00 -multiply_by 2 -divide_by 1 -phase 0.00 -master_clock {main_clk_50} [get_pins {sram_pll|altpll_component|auto_generated|pll1|clk[0]}]
create_generated_clock -name {sram_clk_100_20} -source [get_ports {CLOCK_50}] -duty_cycle 25.00 -multiply_by 2 -divide_by 1 -phase -90.00 -master_clock {main_clk_50} [get_pins {sram_pll|altpll_component|auto_generated|pll1|clk[1]}]

#create_generated_clock -name {nios_system|sdram_pll|sd1|pll7|clk[0]} -source [get_pins {nios_system|sdram_pll|sd1|pll7|inclk[0]}] -duty_cycle 50.00 -multiply_by 1 -phase 0.00 -master_clock {main_clk_50} [get_pins {nios_system|sdram_pll|sd1|pll7|clk[0]}] 
#create_generated_clock -name {nios_system|sdram_pll|sd1|pll7|clk[1]} -source [get_pins {nios_system|sdram_pll|sd1|pll7|inclk[0]}] -duty_cycle 50.00 -multiply_by 1 -phase -54.00 -master_clock {main_clk_50} [get_pins {nios_system|sdram_pll|sd1|pll7|clk[1]}] 


#**************************************************************
# Set Clock Latency
#**************************************************************



#**************************************************************
# Set Clock Uncertainty
#**************************************************************



#**************************************************************
# Set Input Delay
#**************************************************************

#set_input_delay -add_delay -max -clock [get_clocks {nios_system|sdram_pll|sd1|pll7|clk[0]}]  3.00 [get_ports {DRAM_DQ}]
#set_input_delay -add_delay -min -clock [get_clocks {nios_system|sdram_pll|sd1|pll7|clk[0]}]  2.00 [get_ports {DRAM_DQ}]

set_input_delay -add_delay -max -clock [get_clocks {main_clk_50}]  3.00 [get_ports {KEY[*]}]
set_input_delay -add_delay -min -clock [get_clocks {main_clk_50}]  2.00 [get_ports {KEY[*]}]

#set_input_delay -add_delay -max -clock [get_clocks {main_clk_50}]  3.00 [get_ports {OTG_DATA[*]}]
#set_input_delay -add_delay -min -clock [get_clocks {main_clk_50}]  2.00 [get_ports {OTG_DATA[*]}]

#set_input_delay -add_delay -min -clock [get_clocks {main_clk_50}]  2.00 [get_ports {OTG_INT}]

# set_input_delay -add_delay -max -clock [get_clocks {main_clk_50}]  3.00 [get_ports {altera_reserved_tck}]
# set_input_delay -add_delay -min -clock [get_clocks {main_clk_50}]  2.00 [get_ports {altera_reserved_tck}]
# set_input_delay -add_delay -max -clock [get_clocks {main_clk_50}]  3.00 [get_ports {altera_reserved_tdi}]
# set_input_delay -add_delay -min -clock [get_clocks {main_clk_50}]  2.00 [get_ports {altera_reserved_tdi}]
# set_input_delay -add_delay -max -clock [get_clocks {main_clk_50}]  3.00 [get_ports {altera_reserved_tms}]
# set_input_delay -add_delay -min -clock [get_clocks {main_clk_50}]  2.00 [get_ports {altera_reserved_tms}]

set_input_delay -add_delay -max -clock [get_clocks {sram_clk_100_20}] -clock_fall  6.500 [get_ports {SRAM_DQ[*]}]
set_input_delay -add_delay -min -clock [get_clocks {sram_clk_100_20}] -clock_fall  0.00 [get_ports {SRAM_DQ[*]}]

# set_input_delay -add_delay -min -clock [get_clocks {sram_clk_100_20}] -clock_fall  2.00 [get_ports {altera_reserved_tms}]


#**************************************************************
# Set Output Delay
#**************************************************************

#set_output_delay -add_delay  -clock [get_clocks {nios_system|sdram_pll|sd1|pll7|clk[0]}]  2.00 [get_ports {DRAM_*}]

#set_output_delay -add_delay  -clock [get_clocks {main_clk_50}]  2.00 [get_ports {HEX*}]

#set_output_delay -add_delay  -clock [get_clocks {main_clk_50}]  2.00 [get_ports {OTG_*}]

set_output_delay -add_delay  -clock [get_clocks {vga_clk_25}]  2.00 [get_ports {VGA_*}]

set_output_delay -add_delay  -clock [get_clocks {sram_clk_100_50}] 2.00 [get_ports {SRAM_OE_N}]
set_output_delay -add_delay  -clock [get_clocks {sram_clk_100_50}] 2.00 [get_ports {SRAM_WE_N}]

set_output_delay -add_delay  -clock [get_clocks {sram_clk_100_50}] 5.00 [get_ports {SRAM_ADDR[*]}]


# set_output_delay -add_delay  -clock [get_clocks {main_clk_50}]  2.00 [get_ports {altera_reserved_tdo}]


#**************************************************************
# Set Clock Groups
#**************************************************************

# set_clock_groups -asynchronous -group [get_clocks {altera_reserved_tck}] 
# set_clock_groups -asynchronous -group [get_clocks {altera_reserved_tck}] 


#**************************************************************
# Set False Path
#**************************************************************

#set_false_path -from [get_registers {*altera_avalon_st_clock_crosser:*|in_data_buffer*}] -to [get_registers {*altera_avalon_st_clock_crosser:*|out_data_buffer*}]
#set_false_path -to [get_keepers {*altera_std_synchronizer:*|din_s1}]
#set_false_path -to [get_pins -nocase -compatibility_mode {*|alt_rst_sync_uq1|altera_reset_synchronizer_int_chain*|clrn}]
#set_false_path -from [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_nios2_oci_break:the_lab7_soc_nios2_qsys_0_nios2_oci_break|break_readreg*}] -to [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper:the_lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper|lab7_soc_nios2_qsys_0_jtag_debug_module_tck:the_lab7_soc_nios2_qsys_0_jtag_debug_module_tck|*sr*}]
#set_false_path -from [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_nios2_oci_debug:the_lab7_soc_nios2_qsys_0_nios2_oci_debug|*resetlatch}] -to [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper:the_lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper|lab7_soc_nios2_qsys_0_jtag_debug_module_tck:the_lab7_soc_nios2_qsys_0_jtag_debug_module_tck|*sr[33]}]
#set_false_path -from [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_nios2_oci_debug:the_lab7_soc_nios2_qsys_0_nios2_oci_debug|monitor_ready}] -to [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper:the_lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper|lab7_soc_nios2_qsys_0_jtag_debug_module_tck:the_lab7_soc_nios2_qsys_0_jtag_debug_module_tck|*sr[0]}]
#set_false_path -from [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_nios2_oci_debug:the_lab7_soc_nios2_qsys_0_nios2_oci_debug|monitor_error}] -to [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper:the_lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper|lab7_soc_nios2_qsys_0_jtag_debug_module_tck:the_lab7_soc_nios2_qsys_0_jtag_debug_module_tck|*sr[34]}]
#set_false_path -from [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_nios2_ocimem:the_lab7_soc_nios2_qsys_0_nios2_ocimem|*MonDReg*}] -to [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper:the_lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper|lab7_soc_nios2_qsys_0_jtag_debug_module_tck:the_lab7_soc_nios2_qsys_0_jtag_debug_module_tck|*sr*}]
#set_false_path -from [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper:the_lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper|lab7_soc_nios2_qsys_0_jtag_debug_module_tck:the_lab7_soc_nios2_qsys_0_jtag_debug_module_tck|*sr*}] -to [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper:the_lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper|lab7_soc_nios2_qsys_0_jtag_debug_module_sysclk:the_lab7_soc_nios2_qsys_0_jtag_debug_module_sysclk|*jdo*}]
#set_false_path -from [get_keepers {sld_hub:*|irf_reg*}] -to [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper:the_lab7_soc_nios2_qsys_0_jtag_debug_module_wrapper|lab7_soc_nios2_qsys_0_jtag_debug_module_sysclk:the_lab7_soc_nios2_qsys_0_jtag_debug_module_sysclk|ir*}]
#set_false_path -from [get_keepers {sld_hub:*|sld_shadow_jsm:shadow_jsm|state[1]}] -to [get_keepers {*lab7_soc_nios2_qsys_0:*|lab7_soc_nios2_qsys_0_nios2_oci:the_lab7_soc_nios2_qsys_0_nios2_oci|lab7_soc_nios2_qsys_0_nios2_oci_debug:the_lab7_soc_nios2_qsys_0_nios2_oci_debug|monitor_go}]


#**************************************************************
# Set Multicycle Path
#**************************************************************



#**************************************************************
# Set Maximum Delay
#**************************************************************



#**************************************************************
# Set Minimum Delay
#**************************************************************



#**************************************************************
# Set Input Transition
#**************************************************************

