import sys
import re


def reduce_value(val):
    print("NEXT")
    print(val)
    # first check if explosion necessary
    depth, index_left = 0, 0
    for i in range(len(val)):
        if val[i] == "[":
            index_left = i
            depth += 1
        elif val[i] == "]":
            if depth > 4:
                pair_to_explode = val[index_left:i+1]
                print("pair_to_explode: " + pair_to_explode)

                # search for next number from current pos to add snd val right:
                substr_to_search_right = val[i+1:]
                print("substr_to_search right: " + substr_to_search_right)
                occ_list = [match for match in re.finditer("([0-9]+)", substr_to_search_right)]
                if len(occ_list) > 0:
                    next_val = int(substr_to_search_right[occ_list[0].start():occ_list[0].end()])
                    next_val += int(pair_to_explode.split(",")[1].strip("]"))
                    substr_to_search_right = substr_to_search_right[:occ_list[0].start()] + str(next_val) + substr_to_search_right[occ_list[0].end():]
                    print(substr_to_search_right)

                # search for last number from current pos to add first val left:
                substr_to_search_left = val[:index_left]
                print("substr_to_search_left: " + substr_to_search_left)
                occ_list = [match for match in re.finditer("([0-9]+)", substr_to_search_left)]
                if len(occ_list) > 0:
                    prev_val = int(substr_to_search_left[occ_list[-1].start():occ_list[-1].end()])
                    prev_val += int(pair_to_explode.split(",")[0].strip("["))
                    substr_to_search_left = substr_to_search_left[:occ_list[-1].start()] + str(
                        prev_val) + substr_to_search_left[occ_list[-1].end():]
                    print(substr_to_search_left)
                val = substr_to_search_left + "0" + substr_to_search_right
                # recursive call
                reduce_value(val)

    # no explode necessary, check if split necessary TODO

    # nothing necessary, number is complete
    return val


def snailfish_add(curr, new):
    #retval_before_reduce = [curr, new]

    #regex_explosion_finder = re.compile()
    #egex_split_finder = re.compile()
    return 0


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
    test_cases = {'[[[[[9,8],1],2],3],4]': '[[[[0,9],2],3],4]', '[7,[6,[5,[4,[3,2]]]]]': '[7,[6,[5,[7,0]]]]',
                  '[[6,[5,[4,[3,2]]]],1]': '[[6,[5,[7,0]]],3]',
                  '[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]': '[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]',
                  '[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]': '[[3,[2,[8,0]]],[9,[5,[7,0]]]]',
                  '[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]': '[[[[0,7],4],[[7,8],[6,0]]],[8,1]]'}
    for val, res in test_cases.items():
        #assert reduce_value(val) == res
        reduce_value(val)


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        curr_val = None
        for line in f.readlines():
            curr_val = snailfish_add(curr_val, line.strip())
    #mag = calc_magnitude(curr_val)
    test_magnitude()
    test_reduce()
