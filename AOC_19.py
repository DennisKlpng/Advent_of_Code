import utils
import functools


patterns = []


@functools.lru_cache()
def try_solve(design: str, pt2=False):
    global patterns
    if design == "":
        return 1
    possible = 0
    for pat in patterns:
        if design.startswith(pat):
            if pt2:
                possible += try_solve(design[len(pat):], pt2)
            else:
                possible = max(possible, try_solve(design[len(pat):]), pt2)
    return possible


@utils.time_tracker
def solve_puzzle(filename):
    global patterns
    res_pt1, res_pt2 = 0, 0
    patt_list, designs = utils.split_fileinput_by_emptylines(filename)
    patterns = patt_list[0].split(", ")
    for dsg in designs:
        res_pt1 += try_solve(dsg)
        res_pt2 += try_solve(dsg, True)
    return res_pt1, res_pt2


if __name__ == '__main__':
    p1, p2 = solve_puzzle("inputs/Test_19.txt")
    print(p1, p2)
    p1, p2 = solve_puzzle("inputs/Data_19.txt")
    print(p1, p2)