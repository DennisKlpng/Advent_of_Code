import sys
import re
from itertools import combinations


def reduce_value(val):
    # first check if explosion necessary
    depth, index_left = 0, 0
    for i in range(len(val)):
        if val[i] == "[":
            index_left = i
            depth += 1
        elif val[i] == "]":
            if depth > 4:
                pair_to_explode = val[index_left:i+1]

                # search for next number from current pos to add snd val right:
                substr_to_search_right = val[i+1:]
                occ_list = [match for match in re.finditer("([0-9]+)", substr_to_search_right)]
                if len(occ_list) > 0:
                    next_val = int(substr_to_search_right[occ_list[0].start():occ_list[0].end()])
                    next_val += int(pair_to_explode.split(",")[1].strip("]"))
                    substr_to_search_right = substr_to_search_right[:occ_list[0].start()] + str(next_val) + substr_to_search_right[occ_list[0].end():]

                # search for last number from current pos to add first val left:
                substr_to_search_left = val[:index_left]
                occ_list = [match for match in re.finditer("([0-9]+)", substr_to_search_left)]
                if len(occ_list) > 0:
                    prev_val = int(substr_to_search_left[occ_list[-1].start():occ_list[-1].end()])
                    prev_val += int(pair_to_explode.split(",")[0].strip("["))
                    substr_to_search_left = substr_to_search_left[:occ_list[-1].start()] + str(
                        prev_val) + substr_to_search_left[occ_list[-1].end():]
                # add everything together
                val = substr_to_search_left + "0" + substr_to_search_right
                # recursive call
                val = reduce_value(val)
                break
            else:
                depth -= 1
    # no explode necessary, check if split necessary and split
    occ = re.search('([0-9]{2,})', val)
    if occ:
        value = int(occ.group())
        new_str = val[:occ.span()[0]] + '[' + str(int(value / 2)) + ',' + str(int((value + 1) / 2)) + ']' + val[occ.span()[1]:]
        val = new_str

        val = reduce_value(val)

    # nothing necessary, number is complete
    return val


def snailfish_add(curr, new):
    if not curr:
        return new
    retval_before_reduce = "[" + curr + "," + new + "]"

    return reduce_value(retval_before_reduce)


def calc_magnitude(value):
    retval = 0
    regex = re.compile('([\[{1}])([0-9,]{3,})([\]{1}])')
    while True:
        occ = regex.search(value)
        if not occ:
            retval = int(value)
            break
        temp_vals = [int(x) for x in occ.group()[1:-1].split(',')]
        temp_val = str(3 * temp_vals[0] + 2 * temp_vals[1])
        value = regex.sub(temp_val, value, 1)

    return retval


def test_magnitude():
    test_cases = {'[[1,2],[[3,4],5]]': 143, '[[[[0,7],4],[[7,8],[6,0]]],[8,1]]': 1384,
                  '[[[[1,1],[2,2]],[3,3]],[4,4]]': 445, '[[[[3,0],[5,3]],[4,4]],[5,5]]': 791,
                  '[[[[5,0],[7,4]],[5,5]],[6,6]]': 1137, '[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]': 3488}
    for val, res in test_cases.items():
        assert calc_magnitude(val) == res


def test_reduce():
    test_cases = {#'[[[[[9,8],1],2],3],4]': '[[[[0,9],2],3],4]', '[7,[6,[5,[4,[3,2]]]]]': '[7,[6,[5,[7,0]]]]',
                  #'[[6,[5,[4,[3,2]]]],1]': '[[6,[5,[7,0]]],3]',
                  #'[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]': '[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]',
                  #'[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]': '[[3,[2,[8,0]]],[9,[5,[7,0]]]]',
                  '[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]': '[[[[0,7],4],[[7,8],[6,0]]],[8,1]]'}
    for val, res in test_cases.items():
        print("Testcase " + val)
        assert reduce_value(val) == res
        #reduce_value(val)


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        curr_val = None
        nums = []
        for line in f.readlines():
            curr_val = snailfish_add(curr_val, line.strip())
            nums.append(line.strip())
        mag = calc_magnitude(curr_val)
    print("Pt 1 solution: " + str(mag))
    #test_magnitude()
    #test_reduce()
    mag_max = 0
    list_comb = combinations(nums, 2)
    for comb in list_comb:
        mag_max = max(mag_max, calc_magnitude(snailfish_add(comb[0], comb[1])))
        mag_max = max(mag_max, calc_magnitude(snailfish_add(comb[1], comb[0])))
    print("Pt 2 solution: " + str(mag_max))
