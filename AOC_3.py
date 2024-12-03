import utils
import re

reg_mul = re.compile("mul\(([0-9]{1,3}),([0-9]{1,3})\)")
reg_cond = re.compile("don't\(\).*?(do\(\)|$)", re.DOTALL)


def solve_puzzle(filename):
    input_data = utils.read_file_as_chunk(filename)
    res_1 = [int(x[0]) * int(x[1]) for x in reg_mul.findall(input_data)]
    input_data_mod = reg_cond.sub("", input_data)
    res_2 = [int(x[0]) * int(x[1]) for x in reg_mul.findall(input_data_mod)]
    return sum(res_1), sum(res_2)


if __name__ == '__main__':
    p1, p2 = solve_puzzle("Test_3.txt")
    print(p1, p2)
    p1, p2 = solve_puzzle("Data_3.txt")
    print(p1, p2)
