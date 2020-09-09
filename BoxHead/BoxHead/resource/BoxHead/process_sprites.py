import os
import argparse
from PIL import Image
from progress.bar import Bar


def rgb_to_16bit(rgb: (int, int, int)) -> int:
    return ((rgb[0] & 0xF8) << 8) | ((rgb[1] & 0xFC) << 3) | ((rgb[2] & 0xF8) >> 3)


def revert_to_rgb(color16: int) -> (int, int, int):
    return (color16 & 0xF800) >> 8, (color16 & 0x7E0) >> 3, (color16 & 0x1F) << 3


def hex_to_rgb(h: str) -> (int, int, int):
    return int(h[0:4], 16), int(h[4:6], 16), int(h[6:8], 16)


RESIZE_FACTOR = 0.25
ALPHA_THRESHOLD = 150
TRANSPARENT_COLOR = 0x07E0

FRAMES = {
    "zombie_walk": [1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8],
    "zombie_hit_front": [1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2],
    # "zombie_hit_back": [1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2],
    "zombie_die": [1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5],
    "zombie_attack": [1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4],

    "player_walk": [1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8],
    "player_hit_front": [1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2],
    # "player_hit_back": [1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2],
    "player_die": [1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5],
}

STATIC_IMAGES = [
    "blood_1", "blood_2", "blood_3", "remain_blood", "gun_line", "P1", "P2", "wall"
]

DIRECTIONS = [0, 45, 90, 135, 180, 225, 270, 315]


def map_to_actual_resource(action: str, direction: int) -> (int, int):
    """
    Map a direction to an existing resource
    :param action:
    :param direction:
    :return: (resource_direction, flip_x)
    """
    if os.path.exists(os.path.join(action, str(direction))):
        return direction, 0
    if os.path.exists(os.path.join(action, str(360 - direction))):
        return 360 - direction, 1
    if os.path.exists(os.path.join(action, str(direction - 45))):
        return direction - 45, 0
    if os.path.exists(os.path.join(action, str(360 - (direction - 45)))):
        return 360 - (direction - 45), 1
    raise ValueError("Failed to map to actual resource with %s %d" % (action, direction))


def run() -> None:
    # ================ Process resource and fill OCM buffer ================

    img_data = {}  # action: str -> parameters: dict
    ocm_data = []
    offset = 0

    for action, ids in FRAMES.items():

        action_data = {}  # direction: int -> parameters: dict

        id_start = min(ids)
        assert id_start == 1
        id_end = max(ids)

        for direction in DIRECTIONS:

            dirc_data = {}

            if not os.path.exists(os.path.join(action, str(direction))):  # no actual resource for this direction
                continue

            dirc_data["offset"] = offset  # record offset of first frame

            for img_id in range(id_start, id_end + 1):

                img_filename = os.path.join(action, str(direction), str(img_id)) + ".png"
                out_filename = os.path.join("out", img_filename)
                os.makedirs(os.path.dirname(out_filename), exist_ok=True)

                img = Image.open(img_filename).convert("RGBA")
                img = img.resize((int(img.size[0] * RESIZE_FACTOR), int(img.size[1] * RESIZE_FACTOR)))
                out_img = Image.new('RGB', img.size)

                # Record or check for width and height
                for i, param in enumerate(["width", "height"]):
                    if param not in dirc_data.keys():
                        dirc_data[param] = img.size[i]
                    else:
                        assert dirc_data[param] == img.size[i], "%s inconsistent" % param

                for y in range(img.size[1]):
                    for x in range(img.size[0]):
                        r, g, b, a = img.getpixel((x, y))
                        if a < ALPHA_THRESHOLD:
                            color = TRANSPARENT_COLOR
                        else:
                            color = rgb_to_16bit((r, g, b))

                        ocm_data.append(color)
                        offset += 1

                        out_img.putpixel((x, y), revert_to_rgb(color))

                out_img.save(out_filename)

                action_data[direction] = dirc_data

        img_data[action] = action_data

    # ================ Map to actual direction ================

    for action, _ in FRAMES.items():
        for direction in DIRECTIONS:
            res_direction, flip_x = map_to_actual_resource(action, direction)
            for param in ["width", "height", "offset"]:
                if direction not in img_data[action].keys():
                    img_data[action][direction] = {}
                img_data[action][direction][param] = img_data[action][res_direction][param]
            img_data[action][direction]["flip_x"] = flip_x

    # ================ Process static images ================

    static_img_data = {}  # name: str -> parameters: dict

    for image_name in STATIC_IMAGES:
        img_filename = image_name + ".png"
        out_filename = os.path.join("out", img_filename)

        img = Image.open(img_filename).convert("RGBA")
        img = img.resize((int(img.size[0] * RESIZE_FACTOR), int(img.size[1] * RESIZE_FACTOR)))
        out_img = Image.new('RGB', img.size)

        static_img_data[image_name] = {
            "width" : img.size[0],
            "height": img.size[1],
            "offset": offset
        }

        for y in range(img.size[1]):
            for x in range(img.size[0]):
                r, g, b, a = img.getpixel((x, y))
                if a < ALPHA_THRESHOLD:
                    color = TRANSPARENT_COLOR
                else:
                    color = rgb_to_16bit((r, g, b))

                ocm_data.append(color)
                offset += 1

                out_img.putpixel((x, y), revert_to_rgb(color))

        out_img.save(out_filename)

    # ================ Generate resource.h ================

    def_file = open("resource.h", "w")
    def_file.write("#ifndef RESOURCE_H_\n")
    def_file.write("#define RESOURCE_H_\n")
    def_file.write("\n")

    for action, ids in FRAMES.items():

        for param in ["width", "height", "flip_x"]:
            def_file.write("static int %s_%s[8] = {  // [direction]\n" % (action.upper(), param.upper()))
            def_file.write("    ")
            def_file.write(", ".join([str(img_data[action][direction][param]) for direction in DIRECTIONS]))
            def_file.write("\n};\n")

        def_file.write("#define %s_FRAME_COUNT %d\n" % (action.upper(), len(ids)))
        lines = []
        for direction in DIRECTIONS:

            frame_offset_str = []
            for img_id in ids:
                dirc_data = img_data[action][direction]
                frame_offset_str.append(
                    "0x%05X" % (dirc_data["offset"] + (img_id - 1) * dirc_data["width"] * dirc_data["height"])
                )

            lines.append("    {%s}" % (", ".join(frame_offset_str)))

        def_file.write("static int %s_OFFSET[8][%d] = {  // [direction][frame_index]\n" % (action.upper(), len(ids)))
        def_file.write(",\n".join(lines))
        def_file.write("\n};\n")
        def_file.write("\n\n")

    for image_name in STATIC_IMAGES:
        def_file.write("#define %s_WIDTH %d\n" % (image_name.upper(), static_img_data[image_name]["width"]))
        def_file.write("#define %s_HEIGHT %d\n" % (image_name.upper(), static_img_data[image_name]["height"]))
        def_file.write("#define %s_OFFSET 0x%05X\n" % (image_name.upper(), static_img_data[image_name]["offset"]))
        def_file.write("\n")

    def_file.write("#endif  // RESOURCE_H_\n")
    def_file.close()

    # ================ Output OCM ================
    with open("ocm.txt", "w") as ocm_file:
        for i in range(len(ocm_data)):
            ocm_file.write("%04X " % ocm_data[i])

    # ================ Output information ================
    print(f"Total OCM usage: {len(ocm_data) * 16:,} bits")
    print(f"OCM array size: {len(ocm_data)}")
    print(f"OCM array upper: {len(ocm_data) - 1}")


if __name__ == '__main__':
    run()
