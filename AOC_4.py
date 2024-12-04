import utils
import numpy as np
from itertools import product, combinations

def solve_puzzle(filename):
    input_data_array = np.array([[char for char in line] for line in utils.read_file_as_lines(filename)])
    s_loc = np.argwhere(input_data_array == "S")
    neighbour_vecs = list(product([0, 1, -1], [0, 1, -1]))
    occ_xmas, occ_x_mas = 0, 0
    x_max, y_max = np.shape(input_data_array)
    mas_pos = []
    for pos in s_loc:
        for vec in neighbour_vecs:
            if not (0 <= pos[0] - 2*vec[0] < x_max and 0 <= pos[1] - 2*vec[1] < y_max):
                continue  # m would be out of bounds
            if not input_data_array[pos[0] - vec[0]][pos[1] - vec[1]] == "A":
                continue
            if not input_data_array[pos[0] - 2*vec[0]][pos[1] - 2*vec[1]] == "M":
                continue
            new_mas = [[pos[0] - 2 * vec[0], pos[1] - 2 * vec[1]],  # M
                       [pos[0] - vec[0], pos[1] - vec[1]],  # A
                       [pos[0], pos[1]]]  # S
            mas_pos.append(new_mas)
            if not (0 <= pos[0] - 3*vec[0] < x_max and 0 <= pos[1] - 3*vec[1] < y_max):
                continue  # x would be out of bounds
            if not input_data_array[pos[0] - 3*vec[0]][pos[1] - 3*vec[1]] == "X":
                continue
            occ_xmas += 1

    # remove all MAS that are not diagonal (since either x or y coordinate of M and A are identical)
    mas_pos = [x for x in mas_pos if not (x[0][0] == x[1][0] or x[0][1] == x[1][1])]
    combines = list(combinations(mas_pos, 2))
    mas_array = np.full(np.shape(input_data_array), ".")
    for comb in combines:
        if comb[0][1] == comb[1][1]:
            occ_x_mas += 1
    #         mas_array[comb[0][0][0]][comb[0][0][1]] = "M"
    #         mas_array[comb[1][0][0]][comb[1][0][1]] = "M"
    #         mas_array[comb[0][1][0]][comb[0][1][1]] = "A"
    #         mas_array[comb[1][1][0]][comb[1][1][1]] = "A"
    #         mas_array[comb[0][2][0]][comb[0][2][1]] = "S"
    #         mas_array[comb[1][2][0]][comb[1][2][1]] = "S"
    # print(mas_array)

    return occ_xmas, occ_x_mas


if __name__ == '__main__':
    p1, p2 = solve_puzzle("Test_4.txt")
    print(p1, p2)
    p1, p2 = solve_puzzle("Data_4.txt")
    print(p1, p2)
