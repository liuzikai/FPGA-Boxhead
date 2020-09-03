import os
from PIL import Image
import argparse

def convert(input_filename : str, output_filename: str) -> None:
    img = Image.open(input_filename).convert("RGBA")
    with open(output_filename, "w") as out:
        for y in range(img.size[1]):
            for x in range(img.size[0]):
                r, g, b, a = img.getpixel((x,y))
                print("%x, %x, %x, %x" % (r, g, b, a))
                if (a == 0):
                    out.write("07E0\n")
                else:
                    out.write("%x\n" % (((r & 0x1F) << 11) | ((g % 0x3F) << 5) | (b & 0x1F)))

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("image", help="Input image file")
    args = parser.parse_args()
    convert(args.image, os.path.splitext(args.image)[0] + ".txt")