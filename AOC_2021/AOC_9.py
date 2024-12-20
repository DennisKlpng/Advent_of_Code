import sys
import numpy as np


def search_basin_parts(matrix, minima):
    basin_sizes = []

    def search_neighbors(x, y, current):
        if x not in range(len(matrix)) or y not in range(len(matrix[0])) or matrix[x][y] == 9 \
                or matrix[x][y] < current or (x, y) in marked:
            return
        marked.add((x, y))
        elementary_vectors = ((-1, 0), (1, 0), (0, -1), (0, 1))
        current_val = matrix[x][y]
        for vector in elementary_vectors:
            search_neighbors(x + vector[0], y + vector[1], current_val)

    for minimum in minima:
        x = minimum[0]
        y = minimum[1]
        marked = set()
        search_neighbors(x, y, matrix[x][y])
        basin_sizes.append(len(marked))
    basin_sizes.sort(reverse=True)

    return basin_sizes


def get_min_values(matrix):
    dim_columns = len(matrix[0])
    dim_lines = len(matrix)
    coords=[]
    for x in range(dim_lines):
        for y in range(dim_columns):
            up = left = down = right = 10
            if x > 0:
                up = matrix[x-1][y]
            if x < dim_lines - 1:
                down = matrix[x+1][y]
            if y > 0:
                left = matrix[x][y-1]
            if y < dim_columns - 1:
                right = matrix[x][y+1]
            if matrix[x][y] < min(up, left, down, right):
                coords.append([x, y])
    return coords


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        matrix = []
        for line in f:
            line_list = []
            for i in range(len(line.strip())):
                line_list.append(int(line[i].strip()))
            matrix.append(line_list)

        risk = 0
        coords_minima = get_min_values(matrix)
        for entry in coords_minima:
            risk += (1 + matrix[entry[0]][entry[1]])
        print("Risk: " + str(risk))
        basins = search_basin_parts(matrix, coords_minima)
        print("Basin product: " + str(basins[0] * basins[1] * basins[2]))



