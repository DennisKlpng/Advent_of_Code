import sys
import numpy as np


def check_if_winner(board):
    if 0 in np.sum(board, axis=0) or 0 in np.sum(board, axis=1):
        return sum(sum(board))
    return 0


def calc_bingo(fields, input_sequence):
    fields_len = len(fields)
    wins = [0] * fields_len
    for number in input_sequence:
        for n_board in range(fields_len):
            ret_val = [x for x in zip(*np.where(fields[n_board] == number))]
            if len(ret_val) > 0:
                fields[n_board][ret_val[0][0]][ret_val[0][1]] = 0
                board_sum = check_if_winner(fields[n_board])
                if board_sum > 0:  # board fulfills the condition
                    wins[n_board] = 1
                    if sum(wins) == 1:
                        print("Result pt 1: " + str(board_sum * number))
                    if sum(wins) == fields_len:
                        print("Result pt 2: " + str(board_sum * number))
                        return


if __name__ == '__main__':
    # binary solution:
    with open(sys.argv[1], "r") as f:
        blocks = f.read().split("\n\n")
        puzzle_input = [int(x) for x in blocks[0].split(",")]

        bingo_fields = list()
        for i in range(len(blocks) - 1):
            matrix = np.zeros((5, 5))
            lines = blocks[i+1].split("\n")
            for j in range(len(lines)):
                numbers = [x for x in lines[j].split(" ") if x]
                for k in range(len(lines)):
                    matrix[j][k] = int(numbers[k].strip())
            bingo_fields.append(matrix)

        calc_bingo(bingo_fields, puzzle_input)
