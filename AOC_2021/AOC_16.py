import sys
from functools import reduce


conversion = {'0': '0000', '1': '0001', '2': '0010', '3': '0011', '4': '0100', '5': '0101', '6': '0110',
              '7': '0111', '8': '1000', '9': '1001', 'A': '1010', 'B': '1011', 'C': '1100', 'D': '1101',
              'E': '1110', 'F': '1111'}


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        input_codes = f.read().split('\n\n')
        for input_code in input_codes:
            binary_input_code = ""
            input_code = input_code.strip()
            for i in range(len(input_code)):
                binary_input_code += conversion[input_code[i]]
            binary_input_code = binary_input_code

            def parse_package(start, version_sum):
                # get version
                version = int(binary_input_code[start:start + 3], 2)
                version_sum += version
                start += 3
                pkg_type = int(binary_input_code[start:start + 3], 2)
                start += 3
                val = -1
                if 4 == pkg_type:
                    start, val = parse_literal_package(start)
                else:
                    ret = parse_operator_package(start, version_sum, pkg_type)
                    start = ret[0]
                    version_sum = ret[1]
                    val = ret[2]
                return start, version_sum, val

            def parse_literal_package(start):
                num = ""
                while True:
                    cont_bit = binary_input_code[start]
                    num += binary_input_code[start + 1:start + 5]
                    start += 5
                    if "0" == cont_bit:
                        break
                return start, int(num, 2)

            def parse_operator_package(start, version_sum, type):
                length_type = int(binary_input_code[start], 2)
                start += 1
                values = []
                result = 0
                if 0 == length_type:
                    len_pkg = int(binary_input_code[start:start + 15], 2)
                    start += 15
                    tgt = start + len_pkg
                    while start < tgt:
                        # parse subs
                        ret = parse_package(start, version_sum)
                        start = ret[0]
                        version_sum = ret[1]
                        values.append(ret[2])
                else:
                    num_pkg = int(binary_input_code[start:start + 11], 2)
                    start += 11
                    # parse subs
                    for i in range(num_pkg):
                        ret = parse_package(start, version_sum)
                        start = ret[0]
                        version_sum = ret[1]
                        values.append(ret[2])
                if 0 == type:
                    result = sum(values)
                elif 1 == type:
                    result = reduce((lambda x, y: x*y), values)
                elif 2 == type:
                    result = min(values)
                elif 3 == type:
                    result = max(values)
                elif 5 == type:
                    result = 1 if values[0] > values[1] else 0
                elif 6 == type:
                    result = 1 if values[0] < values[1] else 0
                elif 7 == type:
                    result = 1 if values[0] == values[1] else 0
                if result == 1291129408:
                    print(values)
                return start, version_sum, result

            start, version_sum_all = 0, 0
            ret = parse_package(start, version_sum_all)
            start = ret[0]
            version_sum_all = ret[1]

            print("Complete version: " + str(version_sum_all))
            print("Return val: " + str(ret[2]))
