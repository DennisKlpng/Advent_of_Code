import utils
import functools


patterns = []


@functools.lru_cache()
def try_solve(design: str):
    global patterns
    if design == "":
        return True
    possible = False
    for pat in patterns:
        size = len(pat)
        if design.startswith(pat):
            possible = possible | try_solve(design[size:])
    return possible


@utils.time_tracker
def solve_puzzle(filename):
    global patterns
    res_pt1 = 0
    patt_list, designs = utils.split_fileinput_by_emptylines(filename)
    patterns = patt_list[0].split(", ")
    for dsg in designs:
        if try_solve(dsg):
            res_pt1 += 1
    return res_pt1, 0


if __name__ == '__main__':
    p1, p2 = solve_puzzle("inputs/Test_19.txt")
    print(p1, p2)
    p1, p2 = solve_puzzle("inputs/Data_19.txt")
    print(p1, p2)