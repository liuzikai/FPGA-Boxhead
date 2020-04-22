import re

header_p = re.compile(r"module (.+) ?\((.+)\)")
param_p = re.compile(r"(input|output|inout) ?(logic |wire )?(\[[0-9:]+\] )?(.+)")

s = """module lab8_soc (
		input  wire        clk_clk,                //             clk.clk
		input  wire [3:0]  key_wire_export,        //        key_wire.export
		output wire [7:0]  keycode_export,         //         keycode.export
		output wire [1:0]  otg_hpi_address_export, // otg_hpi_address.export
		output wire        otg_hpi_cs_export,      //      otg_hpi_cs.export
		input  wire [15:0] otg_hpi_data_in_port,   //    otg_hpi_data.in_port
		output wire [15:0] otg_hpi_data_out_port,  //                .out_port
		output wire        otg_hpi_r_export,       //       otg_hpi_r.export
		output wire        otg_hpi_reset_export,   //   otg_hpi_reset.export
		output wire        otg_hpi_w_export,       //       otg_hpi_w.export
		input  wire        reset_reset_n,          //           reset.reset_n
		output wire        sdram_clk_clk,          //       sdram_clk.clk
		output wire [12:0] sdram_wire_addr,        //      sdram_wire.addr
		output wire [1:0]  sdram_wire_ba,          //                .ba
		output wire        sdram_wire_cas_n,       //                .cas_n
		output wire        sdram_wire_cke,         //                .cke
		output wire        sdram_wire_cs_n,        //                .cs_n
		inout  wire [31:0] sdram_wire_dq,          //                .dq
		output wire [3:0]  sdram_wire_dqm,         //                .dqm
		output wire        sdram_wire_ras_n,       //                .ras_n
		output wire        sdram_wire_we_n         //                .we_n
	);
"""

# Remove comments and merge lines
lines = s.splitlines()
for i in range(len(lines)):
    idx = lines[i].find("//")
    if idx != -1:
        lines[i] = lines[i][:idx]
s = " ".join(lines)

# Remove duplicate spaces
s = re.sub(r"\s+", " ", s)

# print(s)

args = re.match(header_p, s)

if args:
    result = {
        "Module": args.group(1),
        "Input": [],
        "Output": [],
        "Inout": [],
        "Description": "TODO",
        "Purpose": "TODO"
    }

    params = args.group(2).split(",")
    param_type = None
    param_width = None
    for param in params:
        param = param.strip()

        # print(param)

        fields = re.match(param_p, param)
        if fields:
            param_type = fields.group(1)
            param_width = fields.group(3)
            param_name = fields.group(4)
        else:
            # Use last param_type and param_width
            param_name = param
        
        # print(param_type)
        # print(param_width)
        # print(param_name)
        
        if param_width == None:
            result[param_type.capitalize()].append(param_name)
        else:
            result[param_type.capitalize()].append(param_width + param_name)

    result["Input"] = ", ".join(result["Input"])
    result["Output"] = ", ".join(result["Output"])
    result["Inout"] = ", ".join(result["Inout"])

    print("/**")
    for key, value in result.items():
        if value != "":
            print(" * %s: %s" % (key, value))
    print(" */")

else:
    print("Invalid module header!")