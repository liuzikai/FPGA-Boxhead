import os
from PIL import Image
import argparse

def convert(input_filename : str, output_filename: str) -> None:
    img = Image.open(input_filename).convert("RGBA")
    # outImg = Image.new('RGB', img.size, color='white')
    with open(output_filename, "w") as out:
        for y in range(img.size[1]):
            for x in range(img.size[0]):

                # pixel = img.getpixel((x,y))
                # print(pixel)
                # outImg.putpixel((x,y), pixel)

                r, g, b, a = img.getpixel((x,y))
                print("%02x, %02x, %02x, %02x" % (r, g, b, a))

                # if (a == 0):
                #     out.write("07E0\n")
                # else:
                out.write("%04x\n" % (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3)))
    
    # outImg.save(os.path.splitext(args.image)[0] + "_restore.png")

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("image", help="Input image file")
    args = parser.parse_args()
    convert(args.image, os.path.splitext(args.image)[0] + ".txt")