import sys
import utils
from collections import Counter


def solve_puzzle(filename):
    data = utils.read_file_as_chunk(filename)
    orig_lists = [[int(y) for y in x.split()] for x in data.splitlines()]
    list1, list2 = zip(*orig_lists)
    list1 = list(list1)
    list2 = list(list2)
    counts = Counter(list2)
    list_similars = [x*counts[x] for x in list1]
    sum_sims = sum(list_similars)
    list1.sort()
    list2.sort()
    list_diffs = [abs(x - y) for x, y in zip(list1, list2)]
    sum_diffs = sum(list_diffs)
    return sum_diffs, sum_sims


if __name__ == '__main__':
    print(solve_puzzle("Test_1.txt"))
    print(solve_puzzle("Data_1.txt"))

