import sys


def calc_position(line_list_in, consider_aim):
    depth = 0
    pos = 0
    aim = 0
    for line in line_list_in:
        val = int(line[1])
        if line[0].startswith("f"):
            pos += int(line[1])
            if consider_aim:
                depth += aim * val
        else:
            if consider_aim:
                aim += val if line[0].startswith("d") else -val
            else:
                depth += val if line[0].startswith("d") else -val

    return depth * pos


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        line_list = [line.split() for line in f.read().splitlines()]
        print("Result product Part 1: " + str(calc_position(line_list, False)))
        print("Result product Part 2: " + str(calc_position(line_list, True)))
