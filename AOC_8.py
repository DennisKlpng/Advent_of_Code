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
            output_numbers = parts[1].split(" ")
            number_assoc = [-1] * 10  # number_assoc[n] = number, with n = n-th entry in input_numbers for quick access

            for i in range(10):
                if len(input_numbers[i]) == 2:
                    number_assoc[1] = i
                elif len(input_numbers[i]) == 3:
                    number_assoc[7] = i
                elif len(input_numbers[i]) == 4:
                    number_assoc[4] = i
                elif len(input_numbers[i]) == 7:
                    number_assoc[8] = i

            # The only element present in 7, but not in 1 is the real letter "a" (top)
            assoc_dict["a"] = str_substraction(input_numbers[number_assoc[7]], input_numbers[number_assoc[1]])

            # Substract all unknown elements from "1" => we will have two occurences of "c" missing (these are 5 and 6)
            # and one occurence of "f" missing => this is 2 and we now know "f" and (from "1") "c"
            entries_miss = {input_numbers[number_assoc[1]][0]: 0, input_numbers[number_assoc[1]][1]: 0}
            entry_miss_assoc = {input_numbers[number_assoc[1]][0]: [], input_numbers[number_assoc[1]][1]: []}
            for i in range(10):
                retval = str_substraction(input_numbers[number_assoc[1]], input_numbers[i])
                if retval:
                    entries_miss[retval] += 1
                    entry_miss_assoc[retval].append(i)
            for letter_str, num_assoc in entries_miss.items():
                if num_assoc == 1:
                    assoc_dict["f"] = letter_str
                    number_assoc[2] = entry_miss_assoc[letter_str][0]
                else:
                    assoc_dict["c"] = letter_str
            # the larger of 5 and 6 is 6, the difference between 5 and 6 is e
            if len(input_numbers[entry_miss_assoc[assoc_dict["c"]][0]]) > \
                    len(input_numbers[entry_miss_assoc[assoc_dict["c"]][1]]):
                # entry 0 is 6 and 1 is 5
                number_assoc[6] = entry_miss_assoc[assoc_dict["c"]][0]
                number_assoc[5] = entry_miss_assoc[assoc_dict["c"]][1]
            else:
                number_assoc[6] = entry_miss_assoc[assoc_dict["c"]][1]
                number_assoc[5] = entry_miss_assoc[assoc_dict["c"]][0]
            assoc_dict["e"] = str_substraction(input_numbers[number_assoc[6]], input_numbers[number_assoc[5]])

            # b, d, g still missing; 0, 3, 9 still missing.
            # We can get 9 from substracting all from 8, when the result is e, this one was 9
            # if it's not but the length is 6, it's 0. if it's 5, it's 3
            for i in range(10):
                diff = str_substraction(input_numbers[number_assoc[8]], input_numbers[i])
                if assoc_dict["e"] == diff:
                    number_assoc[9] = i
                elif len(input_numbers[i]) == 6 and diff != assoc_dict["c"]:
                    number_assoc[0] = i
                    assoc_dict["d"] = diff
                elif len(input_numbers[i]) == 5 and i not in number_assoc:
                    number_assoc[3] = i

            # get the missing letters: b = 9 - 3, g = 9 - 4 but not a
            assoc_dict["b"] = str_substraction(input_numbers[number_assoc[9]], input_numbers[number_assoc[3]])
            retval = str_substraction(input_numbers[number_assoc[9]], input_numbers[number_assoc[4]])
            assoc_dict["g"] = str_substraction(retval, assoc_dict["a"])

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
