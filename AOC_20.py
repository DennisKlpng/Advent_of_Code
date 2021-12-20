import sys
import numpy as np


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        image_code = []
        image_code = [char for char in f.readline()]
        matrix_list = []
        for line in f.read().strip().splitlines():
            line_list = [line[i] for i in range(len(line.strip()))]
            matrix_list.append(line_list)

        matrix_map = np.array(matrix_list)
        tgt_steps = 50

        def get_res_value(rowpos, colpos, orig_map):
            strbinary = ""
            for drow in range(-1, 2):
                for dcol in range(-1, 2):
                    strbinary += ("1" if orig_map[rowpos+drow+1, colpos+dcol+1] == "#" else "0")
            index = int(strbinary, 2)
            return image_code[index]


        def zoom_image(image, outside, num_light_pixels):
            pad_char = image_code[0] if outside or image_code[0] == '.' else image_code[511]
            image = np.pad(image, ((2, 2), (2, 2)), 'constant', constant_values=pad_char)
            dim_row, dim_col = np.shape(image)[0], np.shape(image)[1]
            orig_map = np.pad(image, ((1, 1), (1, 1)), 'constant', constant_values=pad_char)

            for row in range(dim_row):
                for col in range(dim_col):
                    image[row, col] = get_res_value(row, col, orig_map)
                    if image[row, col] == '#':
                        num_light_pixels += 1
            return image, num_light_pixels

        for step in range(tgt_steps):
            num_light_pixels = 0
            matrix_map, num_light_pixels = zoom_image(matrix_map, step % 2 == 1, num_light_pixels)

            #print(matrix_map)
            print("Step " + str(step) + " Num light pixels: " + str(num_light_pixels))




