import utils
import numpy as np
from itertools import product, combinations


@utils.time_tracker
def solve_puzzle(filename):
    input_data_array = np.array([[ord(char) for char in line] for line in utils.read_file_as_lines(filename)])
    ord_m, ord_a, ord_x, ord_s = ord("M"), ord("A"), ord("X"), ord("S")
    s_loc = np.argwhere(input_data_array == ord_s)
    s_loc = [[d + 3 for d in loc] for loc in s_loc]
    input_data_array = np.pad(input_data_array, 3)
    a_loc = set()  # using set to prevent adding A-coordinates twice that are part of an X-MAS
    neighbour_vecs = list(product([0, 1, -1], [0, 1, -1]))
    occ_xmas, occ_x_mas = 0, 0
    x_max, y_max = np.shape(input_data_array)
    for pos in s_loc:
        for vec in neighbour_vecs:
            if not input_data_array[(xa := pos[0] - vec[0]), (ya := pos[1] - vec[1])] == ord_a:
                continue
            if not input_data_array[(xm := pos[0] - 2 * vec[0]), (ym := pos[1] - 2 * vec[1])] == ord_m:
                continue
            if (xa != xm) and (ya != ym):  # only consider cases where A and M are on a diagonal (=> S as well)
                a_loc.add((xa, ya))
            if not input_data_array[pos[0] - 3 * vec[0], pos[1] - 3 * vec[1]] == ord_x:
                continue
            occ_xmas += 1

    # idea for pt2: just check that both diagonals involving an "A" are "M" + "S"
    for a in a_loc:
        if input_data_array[a[0] - 1, a[1] + 1] + input_data_array[a[0] + 1, a[1] - 1] == (ord_m + ord_s) and \
                input_data_array[a[0] - 1, a[1] - 1] + input_data_array[a[0] + 1, a[1] + 1] == (ord_m + ord_s):
            occ_x_mas += 1

    return occ_xmas, occ_x_mas


if __name__ == '__main__':
    p1, p2 = solve_puzzle("Test_4.txt")
    print(p1, p2)
    p1, p2 = solve_puzzle("Data_4.txt")
    print(p1, p2)
