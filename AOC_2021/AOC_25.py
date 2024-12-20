import sys
import numpy as np
from copy import deepcopy


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        puzzle_input = []
        for line in f.readlines():
            puzzle_input_line = []
            for i in range(len(line.strip())):
                if line[i] == ">":
                    puzzle_input_line.append(1)
                elif line[i] == "v":
                    puzzle_input_line.append(2)
                else:
                    puzzle_input_line.append(0)
            puzzle_input.append(puzzle_input_line)
        array_input = np.array(puzzle_input)

        num_steps = 0
        num_lines, num_col = array_input.shape[0], array_input.shape[1]
        while True:
            num_steps += 1
            num_moves = 0
            array_input_step = deepcopy(array_input)
            # east moving
            for col in range(num_col):
                for line in range(num_lines):
                    if array_input[line, col] == 1:
                        if array_input[line, (col + 1) % num_col] == 0:
                            num_moves += 1
                            array_input_step[line, col] = 0
                            array_input_step[line, (col + 1) % num_col] = 1
            array_input = array_input_step
            array_input_step = deepcopy(array_input)

            for col in range(num_col):
                for line in range(num_lines):
                    if array_input[line, col] == 2:
                        if array_input[(line + 1) % num_lines, col] == 0:
                            num_moves += 1
                            array_input_step[line, col] = 0
                            array_input_step[(line + 1) % num_lines, col] = 2
            array_input = array_input_step

            if num_moves == 0:
                print("Num steps static: " + str(num_steps))
                break

