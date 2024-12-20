import utils
import numpy as np

global input_data_array


def hike(start_pos, is_part1=True):
    visited = [start_pos]
    counter = 0
    global input_data_array
    for pos in visited:
        height = input_data_array[pos]
        for neighbour_pos in utils.get_neighbours(pos):
            if input_data_array[neighbour_pos] == height + 1:
                if is_part1:
                    # only visit distinct paths
                    if neighbour_pos not in visited:
                        visited.append(neighbour_pos)
                        if height + 1 == 9:
                            counter += 1
                else:
                    # visit all paths
                    visited.append(neighbour_pos)
                    if height + 1 == 9:
                        counter += 1
    return counter


@utils.time_tracker
def solve_puzzle(filename):
    global input_data_array
    input_data_array = np.array([[int(char) for char in line] for line in utils.read_file_as_lines(filename)])
    trailheads = [(x[0]+1, x[1]+1) for x in np.argwhere(input_data_array == 0)]
    input_data_array = np.pad(input_data_array, 1)
    p1, p2 = 0, 0
    for pos in trailheads:
        p1 += hike(pos)
        p2 += hike(pos, False)
    return p1, p2


if __name__ == '__main__':
    p1, p2 = solve_puzzle("inputs/Test_10.txt")
    print(p1, p2)
    p1, p2 = solve_puzzle("inputs/Data_10.txt")
    print(p1, p2)