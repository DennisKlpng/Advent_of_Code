import sys


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        closing_braces = {")": 3, "]": 57, "}": 1197, ">": 25137}
        char_assoc = {"(": ")", "[": "]", "{": "}", "<": ">"}
        braces_complete_vals = {"(": 1, "[": 2, "{": 3, "<": 4}
        sum_value_pt1 = 0
        scores_pt2 = []
        for line in f:
            line_list = list(line.strip())
            i = -1
            while i < len(line_list):
                i += 1
                try:
                    if line_list[i] not in closing_braces:
                        continue
                    if line_list[i] == char_assoc[line_list[i-1]]:
                        line_list.pop(i)
                        line_list.pop(i-1)
                        i -= 2
                        continue
                    sum_value_pt1 += closing_braces[line_list[i]]
                    break
                except IndexError:
                    # reached end of line, line is not corrupted
                    val = 0
                    for entry in reversed(line_list):
                        val = val * 5 + braces_complete_vals[entry]
                    scores_pt2.append(val)

        print("End result pt 1: " + str(sum_value_pt1))
        scores_pt2.sort()
        print("End result pt 2: " + str(scores_pt2[int(len(scores_pt2) / 2)]))







