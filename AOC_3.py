import sys


def power_consumption(bin_list, comparison_list):
    gamma_rate, epsilon_rate = 0, 0
    for compare in compare_list:
        res_bit = sum(int(entry, 2) & compare for entry in bin_list)  # bitwise compare and sum all occurences
        if res_bit / compare >= 0.5 * len(bin_list):  # divide by compare, since int representation
            gamma_rate += compare
        else:
            epsilon_rate += compare
    print("Power consumption: " + str(gamma_rate * epsilon_rate))


def keep_uncommon_bit(input_list, compare_bit):
    if len(input_list) == 1:
        return input_list[0] & compare_bit, [input_list[0] & (compare_bit - 1)]
    res_bit = 0 if sum(entry & compare_bit for entry in input_list) / compare_bit >= 0.5 * len(
        input_list) else compare_bit
    ret_list = list()
    for entry in input_list:
        if res_bit > 0 and entry & res_bit:
            ret_list.append(entry & (compare_bit - 1))
        elif res_bit == 0 and not entry & compare_bit:
            ret_list.append(entry & (compare_bit - 1))
    return res_bit, ret_list


def keep_common_bit(input_list, compare_bit):
    if len(input_list) == 1:
        return input_list[0] & compare_bit, [input_list[0] & (compare_bit - 1)]
    res_bit = compare_bit if sum(entry & compare_bit for entry in input_list) / compare_bit >= 0.5 * len(
        input_list) else 0
    ret_list = list()
    for entry in input_list:
        if res_bit > 0 and entry & res_bit:  # common bit is 1 and entry starts with 1
            ret_list.append(entry & (compare_bit - 1))
        elif res_bit == 0 and not entry & compare_bit:  # common bit is 0 and entry starts with 0
            ret_list.append(entry & (compare_bit - 1))
    return res_bit, ret_list


def life_support(bin_list, comparison_list):
    oxygen_rating, co2_rating = 0, 0
    oxygen_list = co2_list = [int(entry, 2) for entry in bin_list]
    for compare in comparison_list:
        oxygen_res = keep_common_bit(oxygen_list, compare)
        oxygen_rating += oxygen_res[0]
        oxygen_list = oxygen_res[1]
        co2_res = keep_uncommon_bit(co2_list, compare)
        co2_rating += co2_res[0]
        co2_list = co2_res[1]

    print("Life Support: " + str(oxygen_rating * co2_rating))


if __name__ == '__main__':
    # binary solution:
    with open(sys.argv[1], "r") as f:
        str_bin_list = ["0B" + line for line in f.read().splitlines()]  # convert input into binary int literal
        compare_list = [pow(2, len(str_bin_list[0])-3-i) for i in range(len(str_bin_list[0]) - 2)]

        power_consumption(str_bin_list, compare_list)
        life_support(str_bin_list, compare_list)
