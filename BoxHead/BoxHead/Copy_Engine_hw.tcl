# TCL File Generated by Component Editor 18.1
# Sun Sep 06 20:19:42 IRKT 2020
# DO NOT MODIFY


# 
# Copy_Engine "Copy Engine" v1.0
# liuzikai 2020.09.06.20:19:42
# A module to copy data from on-chip memory to SRAM
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module Copy_Engine
# 
set_module_property DESCRIPTION "A module to copy data from on-chip memory to SRAM"
set_module_property NAME Copy_Engine
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property GROUP "ECE 385 Custom IPs"
set_module_property AUTHOR liuzikai
set_module_property DISPLAY_NAME "Copy Engine"
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL copy_engine_wrapper
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file copy_engine_wrapper.sv SYSTEM_VERILOG PATH copy_engine_wrapper.sv TOP_LEVEL_FILE
add_fileset_file copy_engine.sv SYSTEM_VERILOG PATH copy_engine.sv

add_fileset SIM_VERILOG SIM_VERILOG "" ""
set_fileset_property SIM_VERILOG TOP_LEVEL copy_engine_wrapper
set_fileset_property SIM_VERILOG ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property SIM_VERILOG ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file copy_engine_wrapper.sv SYSTEM_VERILOG PATH copy_engine_wrapper.sv
add_fileset_file copy_engine.sv SYSTEM_VERILOG PATH copy_engine.sv


# 
# parameters
# 


# 
# display items
# 


# 
# connection point CLK
# 
add_interface CLK clock end
set_interface_property CLK clockRate 50000000
set_interface_property CLK ENABLED true
set_interface_property CLK EXPORT_OF ""
set_interface_property CLK PORT_NAME_MAP ""
set_interface_property CLK CMSIS_SVD_VARIABLES ""
set_interface_property CLK SVD_ADDRESS_GROUP ""

add_interface_port CLK CLK clk Input 1


# 
# connection point RESET
# 
add_interface RESET reset end
set_interface_property RESET associatedClock CLK
set_interface_property RESET synchronousEdges DEASSERT
set_interface_property RESET ENABLED true
set_interface_property RESET EXPORT_OF ""
set_interface_property RESET PORT_NAME_MAP ""
set_interface_property RESET CMSIS_SVD_VARIABLES ""
set_interface_property RESET SVD_ADDRESS_GROUP ""

add_interface_port RESET RESET reset Input 1


# 
# connection point Export_Data
# 
add_interface Export_Data conduit end
set_interface_property Export_Data associatedClock CLK
set_interface_property Export_Data associatedReset RESET
set_interface_property Export_Data ENABLED true
set_interface_property Export_Data EXPORT_OF ""
set_interface_property Export_Data PORT_NAME_MAP ""
set_interface_property Export_Data CMSIS_SVD_VARIABLES ""
set_interface_property Export_Data SVD_ADDRESS_GROUP ""

add_interface_port Export_Data src_data src_data Input 16
add_interface_port Export_Data src_addr src_addr Output 20
add_interface_port Export_Data program_y program_y Output 10
add_interface_port Export_Data program_x program_x Output 10
add_interface_port Export_Data program_write program_write Output 1
add_interface_port Export_Data program_data program_data Output 16
add_interface_port Export_Data palette_index palette_index Output 2
add_interface_port Export_Data current_frame current_frame Input 1
add_interface_port Export_Data engine_done engine_done Output 1
add_interface_port Export_Data engine_execute engine_execute Output 1


# 
# connection point Copy_Engine_Slave_1
# 
add_interface Copy_Engine_Slave_1 avalon end
set_interface_property Copy_Engine_Slave_1 addressUnits WORDS
set_interface_property Copy_Engine_Slave_1 associatedClock CLK
set_interface_property Copy_Engine_Slave_1 associatedReset RESET
set_interface_property Copy_Engine_Slave_1 bitsPerSymbol 8
set_interface_property Copy_Engine_Slave_1 burstOnBurstBoundariesOnly false
set_interface_property Copy_Engine_Slave_1 burstcountUnits WORDS
set_interface_property Copy_Engine_Slave_1 explicitAddressSpan 0
set_interface_property Copy_Engine_Slave_1 holdTime 0
set_interface_property Copy_Engine_Slave_1 linewrapBursts false
set_interface_property Copy_Engine_Slave_1 maximumPendingReadTransactions 0
set_interface_property Copy_Engine_Slave_1 maximumPendingWriteTransactions 0
set_interface_property Copy_Engine_Slave_1 readLatency 0
set_interface_property Copy_Engine_Slave_1 readWaitStates 0
set_interface_property Copy_Engine_Slave_1 readWaitTime 0
set_interface_property Copy_Engine_Slave_1 setupTime 0
set_interface_property Copy_Engine_Slave_1 timingUnits Cycles
set_interface_property Copy_Engine_Slave_1 writeWaitTime 0
set_interface_property Copy_Engine_Slave_1 ENABLED true
set_interface_property Copy_Engine_Slave_1 EXPORT_OF ""
set_interface_property Copy_Engine_Slave_1 PORT_NAME_MAP ""
set_interface_property Copy_Engine_Slave_1 CMSIS_SVD_VARIABLES ""
set_interface_property Copy_Engine_Slave_1 SVD_ADDRESS_GROUP ""

add_interface_port Copy_Engine_Slave_1 AVL_CS chipselect Input 1
add_interface_port Copy_Engine_Slave_1 AVL_BYTE_EN byteenable Input 4
add_interface_port Copy_Engine_Slave_1 AVL_ADDR address Input 4
add_interface_port Copy_Engine_Slave_1 AVL_READ read Input 1
add_interface_port Copy_Engine_Slave_1 AVL_READDATA readdata Output 32
add_interface_port Copy_Engine_Slave_1 AVL_WRITE write Input 1
add_interface_port Copy_Engine_Slave_1 AVL_WRITEDATA writedata Input 32
set_interface_assignment Copy_Engine_Slave_1 embeddedsw.configuration.isFlash 0
set_interface_assignment Copy_Engine_Slave_1 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment Copy_Engine_Slave_1 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment Copy_Engine_Slave_1 embeddedsw.configuration.isPrintableDevice 0

