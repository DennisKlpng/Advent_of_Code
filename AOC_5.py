import sys
import numpy as np


def convert_line(list_input_pts, consider_diag) -> list:
    points = [[int(x) for x in pt.split(",")] for pt in list_input_pts]
    diff_vec = [points[1][0] - points[0][0], points[1][1] - points[0][1]]
    if points[0][0] != points[1][0] and points[0][1] != points[1][1]:  # not x-y axis
        if not consider_diag:
            return None
        elif abs(diff_vec[0]) != abs(diff_vec[1]):  # not diagonal
            return None

    len_diff_vec = max(abs(diff_vec[0]), abs(diff_vec[1]))  # if one is => non-zero value, if diag both equal
    abs_diff_vec = [int(diff_vec[0]/len_diff_vec), int(diff_vec[1]/len_diff_vec)]

    return [[points[0][0] + i*abs_diff_vec[0], points[0][1] + i*abs_diff_vec[1]] for i in range(0, len_diff_vec + 1)]


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        coords = [line.split(" -> ") for line in f.read().splitlines()]
        dim = 10 if len(coords) == 10 else 1000
        map_thermal_smp = np.zeros((dim, dim))
        map_thermal_compl = np.zeros((dim, dim))
        for line in coords:
            output_pts_simple = convert_line(line, False)
            output_pts_complex = convert_line(line, True)
            if output_pts_simple:
                for point in output_pts_simple:
                    map_thermal_smp[point[0]][point[1]] = np.nan if map_thermal_smp[point[0]][point[1]] == 0 else 1
            if output_pts_complex:
                for point in output_pts_complex:
                    map_thermal_compl[point[0]][point[1]] = np.nan if map_thermal_compl[point[0]][point[1]] == 0 else 1

        print("Resulting double points only vert/hor: " + str(int(np.nansum(map_thermal_smp))))
        print("Resulting double points vert/hor/diag: " + str(int(np.nansum(map_thermal_compl))))
