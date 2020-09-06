import os
import argparse
from PIL import Image
from pathlib import Path
from scipy.spatial import KDTree
from progress.bar import Bar

def rgb_to_16bit(rgb: (int, int, int)) -> int:
    return ((rgb[0] & 0xF8) << 8) | ((rgb[1] & 0xFC) << 3) | ((rgb[2] & 0xF8) >> 3)

def revert_to_rgb(color16: int) -> (int, int, int):
    return (color16 & 0xF800) >> 8, (color16 & 0x7E0) >> 3, (color16 & 0x1F) << 3

def hex_to_rgb(h: str) -> (int, int, int):
    return int(h[0:4], 16), int(h[4:6], 16), int(h[6:8], 16)

def enum_files(input_dir: str) -> map:
    ret = {}
    for path in Path(input_dir).rglob("*.png"):
        relpath = str(path.relative_to(input_dir))
        ret[relpath.replace("/", "_")[:-4]] = relpath
    return {k: v for k, v in sorted(ret.items(), key=lambda item: item[1])}

RESIZE_FACTOR = 0.3
ALPHA_THRESHOLD = 150
TRANSPARENT_COLOR = 0x07E0
PALETTE_WIDTH = 16
PALETTE_SIZE = (1 << PALETTE_WIDTH)

def run(input_dir: str) -> None:

    png_files = enum_files(input_dir)
    out_dir = input_dir + "_out"
    os.makedirs(out_dir, exist_ok=True)

    print("Found %d png files" % len(png_files))

    images = {}
    for png_name, png_relpath in png_files.items():
         img = Image.open(os.path.join(input_dir, png_relpath)).convert("RGBA")
         img = img.resize((int(img.size[0] * RESIZE_FACTOR), int(img.size[1] * RESIZE_FACTOR)))
         images[png_name] = img

    color_counts = {}
    for png_name, img in images.items():
         for y in range(img.size[1]):
            for x in range(img.size[0]):
                r, g, b, a = img.getpixel((x,y))
                if a < ALPHA_THRESHOLD:
                    color = TRANSPARENT_COLOR
                else:
                    color = rgb_to_16bit((r, g, b))
                if color not in color_counts.keys():
                    color_counts[color] = 1
                else:
                    color_counts[color] += 1

    palette = [k for k, _ in sorted(color_counts.items(), key=lambda item: item[1])][-PALETTE_SIZE:]

    print("Discovered %d colors, select %d" % (len(color_counts), len(palette)))
    
    # palette_hex = ['0x858585','0xFFFFFF', '0xBCD0EC', '0x5E6876', '0x131313', '0x252525', '0x707070']
    # palette = [rgb_to_16bit(hex_to_rgb(h)) for h in palette_hex] + [TRANSPARENT_COLOR]

    assert len(palette) <= PALETTE_SIZE
    assert TRANSPARENT_COLOR in palette, "Transparent color not in palette"

    with open(os.path.join(out_dir, "palette.txt"), "w") as f:
        f.write("unique case (index)\n")
        for i, color in enumerate(palette):
            f.write("    %2d: color = 16'h%04X;\n" % (i, color))
        f.write("endcase")

    bar = Bar('Processing', max=len(images), suffix='%(percent)d%%')

    palette_rgb = [revert_to_rgb(color) for color in palette]
    kd_tree = KDTree(palette_rgb)

    offset = 0
    offset_file = open(os.path.join(out_dir, "offset.txt"), "w")
    output_file = open(os.path.join(out_dir, "output.txt"), "w")
    buf = 0
    for png_name in images.keys():
        img = images[png_name]
        png_relpath = png_files[png_name]
        out_image = Image.new('RGB', img.size)

        offset_file.write("%8s: %X, %d * %d\n" % (png_name, offset, img.size[0], img.size[1]))

        for y in range(img.size[1]):
            for x in range(img.size[0]):
                r, g, b, a = img.getpixel((x,y))
                if a < ALPHA_THRESHOLD:
                    color = TRANSPARENT_COLOR
                else:
                    color = rgb_to_16bit((r, g, b))
                
                output_file.write("%04X\n" % color)

                out_image.putpixel((x,y), revert_to_rgb(color))

                offset += 1

                # index = kd_tree.query(revert_to_rgb(color))[1]
                
                # buf = buf << 4 | index
                # offset += 1
                # if offset % 4 == 0:
                #     output_file.write("\n")
                #     output_file.write("%04X" % buf)
                #     buf = 0
                
                # output_file.write("%X " % index)
                # offset += 1

                # out_image.putpixel((x,y), revert_to_rgb(palette[index]))

        out_image_filename = os.path.join(out_dir, png_relpath)
        os.makedirs(os.path.dirname(out_image_filename), exist_ok=True)
        out_image.save(out_image_filename)

        bar.next()

    bar.finish()
    offset_file.close()
    output_file.close()

    print(f"In total {offset * PALETTE_WIDTH:,} bits used")

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("input_dir", help="Input image directory")
    args = parser.parse_args()
    run(args.input_dir)