import sys


def str_substraction(string_larger, strings_smaller):
    for character in strings_smaller:
        string_larger = string_larger.replace(character, "")
    return string_larger


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        uniq_length_occ = 0
        output_val = 0
        for line in f.read().splitlines():
            real_numbers = ["abcefg", "cf", "acdeg", "acdfg", "bcdf", "abdfg", "abdefg", "acf", "abcdefg", "abcdfg"]
            assoc_dict = {"a": None, "b": None, "c": None, "d": None,
                          "e": None, "f": None, "g": None}
            parts = line.split(" | ")
            input_numbers = parts[0].split(" ")
            input_numbers.sort(key=lambda x: len(x))
            # sorted list by length => input_list[0] = 1, input_list[1] = 7, input_list[2] = 4, input_list[9] = 8
            output_numbers = parts[1].split(" ")
            number_assoc = [-1, 0, -1, -1, 2, -1, -1, 1, 9, -1]

            # calc complete association
            potential_TwoFive = []
            for i in range(10):
                if len(input_numbers[i]) == 6:  # 0, 6, 9
                    if "" == str_substraction(input_numbers[2], input_numbers[i]):
                        number_assoc[9] = i  # 4-x empty => 9
                    elif "" == str_substraction(input_numbers[1], input_numbers[i]):
                        number_assoc[0] = i  # remainder: only 0 is empty when 7 - x
                    else:
                        number_assoc[6] = i
                elif len(input_numbers[i]) == 5:  # 2, 3, 5
                    if "" == str_substraction(input_numbers[0], input_numbers[i]):
                        number_assoc[3] = i  # 1-x empty => 3
                    else:
                        potential_TwoFive.append(i)

            for i in potential_TwoFive:
                if "" == str_substraction(input_numbers[i], input_numbers[number_assoc[9]]):
                    number_assoc[5] = i  # only 5 is empty, when we substract 9 from it
                else:
                    number_assoc[2] = i

            # calc letter association:
            # a = 7-4; b = 9-3; c = 9-5; d = 8-0; e = 6-5; f = 8-(2+b); g = 9-(4+a)
            assoc_dict["a"] = str_substraction(input_numbers[number_assoc[7]], input_numbers[number_assoc[1]])
            assoc_dict["b"] = str_substraction(input_numbers[number_assoc[9]], input_numbers[number_assoc[3]])
            assoc_dict["c"] = str_substraction(input_numbers[number_assoc[9]], input_numbers[number_assoc[5]])
            assoc_dict["d"] = str_substraction(input_numbers[number_assoc[8]], input_numbers[number_assoc[0]])
            assoc_dict["e"] = str_substraction(input_numbers[number_assoc[6]], input_numbers[number_assoc[5]])

            assoc_dict["f"] = str_substraction(input_numbers[number_assoc[8]], input_numbers[number_assoc[2]]
                                               + assoc_dict["b"])
            assoc_dict["g"] = str_substraction(input_numbers[number_assoc[9]], input_numbers[number_assoc[4]]
                                               + assoc_dict["a"])

            # Convert real numbers with encryption:
            for index in range(10):
                for i in range(len(real_numbers[index])):
                    strlist = list(real_numbers[index])
                    strlist[i] = assoc_dict[strlist[i]]
                    real_numbers[index] = "".join(strlist)

            # Process output
            output_str = ""
            for i in range(4):
                if len(output_numbers[i]) in (2, 3, 4, 7):
                    uniq_length_occ += 1
                for index in range(len(real_numbers)):
                    if "" == str_substraction(real_numbers[index], output_numbers[i]) \
                            and "" == str_substraction(output_numbers[i], real_numbers[index]):
                        output_str += str(index)
            output_val += int(output_str)

        print("Number of unique characters: " + str(uniq_length_occ))
        print("Addition of all numbers: " + str(output_val))
