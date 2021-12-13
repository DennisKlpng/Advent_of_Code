import sys
import os


def fold(point, fold_axis):
    x = y = -1
    fold_info = fold_axis.split("=")
    point_list = point.split(",")
    if fold_info[0] == "x":
        y = int(point_list[1])
        x_in = int(point_list[0])
        x_fold = int(fold_info[1])
        if x_in < x_fold:
            x = x_in
        else:
            x = 2 * x_fold - x_in  # x_fold - (x_in - x_fold)
    else:
        x = int(point_list[0])
        y_in = int(point_list[1])
        y_fold = int(fold_info[1])
        if y_in < y_fold:
            y = y_in
        else:
            y = 2 * y_fold - y_in  # y_fold - (y_in - y_fold)
    return str(str(x) + "," + str(y))


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        input_list_points = {}
        instructions = []
        for line in f:
            if line.strip().startswith("fold"):
                instructions.append(line.strip().split(" ")[2])
            elif line.startswith("\n"):
                continue
            else:
                input_list_points[line.strip()] = 1

        for i in range(len(instructions)):
            output_dict_after_fold = {}
            for entry in input_list_points:
                folded_pt = fold(entry, instructions[i])
                output_dict_after_fold[folded_pt] = 1
            input_list_points = output_dict_after_fold

            if 0 == i:
                print("Part 1 solution: " + str(len(output_dict_after_fold.keys())))

        key_list = [[int(key.split(",")[0]), int(key.split(",")[1])] for key in output_dict_after_fold.keys()]
        x_max = max([x[0] for x in key_list])
        y_max = max([x[1] for x in key_list])

        os.system('color')
        matrix = [[" "]*(x_max+1) for i in range(y_max+1)]
        for entry in key_list:
            matrix[entry[1]][entry[0]] = '\x1b[6;30;42m' + '#' + '\x1b[0m'
        print('\n'.join([''.join([str(cell) for cell in row]) for row in matrix]))
