import sys
import numpy as np


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        input_list = []
        for line in f:
            line_list = []
            for i in range(len(line.strip())):
                line_list.append(int(line[i].strip()))
            input_list.append(line_list)

        matrix_oct = np.array(input_list)
        num_flashes = 0

        def calc_step_for_pos(list_pos):
            flashes_in_step = 0
            list_neighbors_to_trigger = []
            for element in list_pos:
                lin = element[0]
                col = element[1]
                if matrix_flashes_in_step[lin, col] == 0:
                    matrix_oct[lin, col] = 0
                    matrix_flashes_in_step[lin, col] += 1
                    flashes_in_step += 1

                    # get neighbors
                    for i in range(max(lin-1, 0), min(lin+2, 10)):
                        for j in range(max(col-1, 0), min(col+2, 10)):
                            if matrix_flashes_in_step[i, j] == 0:
                                matrix_oct[i, j] += 1  # only increase when it was not flashed (will be 0 anyways)
                            if matrix_oct[i, j] > 9:
                                list_neighbors_to_trigger.append((i, j))
                    # recursive call
                    flashes_in_step += calc_step_for_pos(list_neighbors_to_trigger)

            return flashes_in_step

        step = 0
        while True:
            matrix_flashes_in_step = np.zeros((10, 10))
            matrix_oct += 1
            pos_to_flash = list(zip(*np.where(matrix_oct > 9)))
            num_flashes += calc_step_for_pos(pos_to_flash)

            step += 1
            if step == 100:
                print("Num flashes step 100: " + str(num_flashes))

            if matrix_oct.sum() == 0:
                print("Step of sync: " + str(step))
                break