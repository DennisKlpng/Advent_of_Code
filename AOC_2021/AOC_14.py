import sys


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        conversions = {}
        data = f.read().split("\n\n")
        formula = data[0].strip()
        for line in data[1].split("\n"):
            conv = line.split("->")
            conversions[conv[0].strip()] = conv[1].strip()

        count_elements = dict.fromkeys(conversions.values(), 0)
        count_combines = dict.fromkeys(conversions.keys(), 0)
        for i in range(len(formula)):
            count_elements[formula[i]] += 1
            if i < (len(formula) - 1):
                count_combines[formula[i]+formula[i+1]] += 1

        for i in range(40):
            for combination, count in count_combines.copy().items():
                count_combines[combination] -= count
                letter_to_add = conversions[combination]
                count_elements[letter_to_add] += count
                count_combines[combination[0]+letter_to_add] += count
                count_combines[letter_to_add+combination[1]] += count

        print("Solution: " + str(max(count_elements.values()) - min(count_elements.values())))
