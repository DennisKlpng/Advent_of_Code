import sys
import re


def snailfish_add(curr, new):
    retval = None
    return retval


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
        print("Test mag, expected res: " + str(res))
        assert calc_magnitude(val) == res


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        curr_val = None
        for line in f.readlines():
            curr_val = snailfish_add(curr_val, line.strip())
    #mag = calc_magnitude(curr_val)
    test_magnitude()
