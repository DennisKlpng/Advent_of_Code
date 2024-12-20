import sys


def get_increases_sliding(depth_list, window_size):
    inc_count = 0
    previous_avg_depth = 0
    for i in range(len(depth_list) - (window_size - 1)):
        current_depth = sum(depth_list[i:i+window_size]) / window_size
        if 0 < previous_avg_depth < current_depth:
            inc_count += 1
        previous_avg_depth = current_depth
    return inc_count


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        depths = [int(str_value) for str_value in f.read().splitlines()]

    print("num of increases simple: " + str(get_increases_sliding(depths, 1)))
    print("num of increases sliding: " + str(get_increases_sliding(depths, 3)))
