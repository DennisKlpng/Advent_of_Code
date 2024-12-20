import utils
import functools
from collections import defaultdict
from copy import deepcopy

@functools.lru_cache()
def split(num):
    if num == 0:
        return [1]
    if (x := len(str(num))) % 2 == 0:
        return [int(str(num)[0:int(x/2)]), int(str(num)[int(x/2):])]
    return [num*2024]

@utils.time_tracker
def solve_puzzle(filename):
    nums = [x for x in utils.extract_int_list_from_string(utils.read_file_as_lines(filename)[0])]
    dict_nums = defaultdict(int)
    for n in nums:
        dict_nums[n] += 1
    pt_1 = 0
    for i in range(75):
        new_dict = defaultdict(int)
        for key, value in dict_nums.items():
            nums_new = split(key)
            for n in nums_new:
                new_dict[n] += value
        dict_nums = new_dict
        if i == 24:
            pt_1 = sum(list(dict_nums.values()))

    return pt_1, sum(list(dict_nums.values()))


if __name__ == '__main__':
    p1, p2 = solve_puzzle("inputs/Test_11.txt")
    print(p1, p2)
    p1, p2 = solve_puzzle("inputs/Data_11.txt")
    print(p1, p2)