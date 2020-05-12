import re

header_p = re.compile(r"module (.+) ?\((.+)\)")
param_p = re.compile(r"(input|output|inout) ?(logic |wire )?(\[[0-9:]+\] )?(.+)")

s = """module AddRoundKey (
    input logic [127:0] data_in, 
    input logic [127:0] round_key, 
    output logic [127:0] data_out
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