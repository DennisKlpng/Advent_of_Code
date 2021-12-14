import sys
from collections import Counter


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        conversions = {}
        data = f.read().split("\n\n")
        formula = data[0].strip()
        init_length = len(formula)
        for line in data[1].split("\n"):
            conv = line.split("->")
            conversions[conv[0].strip()] = conv[1].strip()

        for i in range(10):
            expected_size = (init_length - 1)*pow(2, i+1) + 1
            print("expected size: " + str(expected_size))
            for index in range(1, expected_size, 2):
            #    print("insert in: " + formula[index-1:index+1])
                formula = formula[:index] + conversions[formula[index-1:index+1]] + formula[index:]
            #print(formula)

        data = Counter(list(formula))
        print("Solution pt1: " + str(data.most_common()[0][1] - data.most_common()[-1][1]))
