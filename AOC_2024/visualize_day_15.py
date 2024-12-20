from PIL import Image, ImageDraw
import imageio.v3 as iio
import os

scale = 20
x_max_orig = 20
y_max_orig = 10


def create_image(filename):
    img = Image.new(mode="RGB", size=(x_max_orig * scale, y_max_orig * scale),
                    color=(255, 255, 255))
    draw = ImageDraw.Draw(img)
    with open(filename, "r") as f:
        lines = f.read().splitlines()
    for y in range(len(lines)):
        for x in range(len(lines[0])):
            draw.rectangle(xy=(x * scale, y * scale, x * scale + 20, y * scale + 20),
                           fill=(255, 255, 255),
                           outline=(155, 155, 155),
                           width=1)
            c = lines[y][x]
            if c == '#':  # border
                draw.rectangle(xy=(x * scale, y * scale, x * scale + 20, y * scale + 20),
                               fill=(0, 0, 0))
            elif c == '[' or c == ']':
                draw.rectangle(xy=(x * scale + 2, y * scale + 2, x * scale + 18, y * scale + 18),
                               fill=(87, 65, 47),
                               outline=(0, 0, 0),
                               width=2)
            elif c == '@':
                draw.ellipse(xy=(x * scale, y * scale, x * scale + 20, y * scale + 20),
                             fill=(0, 127, 0))
    return img


files = os.listdir(os.getcwd() + "/steps")
name = "abc_34.txt"
files.sort(key=lambda f: int(''.join(filter(str.isdigit, f))))
imgs = [create_image(os.getcwd() + "/steps/" + file) for file in files]
fp_out = os.getcwd() + "/test.gif"

img = imgs[0]
img.save(fp=fp_out, format='GIF', append_images=imgs[1:],
         save_all=True, duration=100, loop=0)

