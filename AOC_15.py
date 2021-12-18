import sys
import numpy as np
import math
import heapq


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        input_data = []
        for line in f.read().splitlines():
            input_data.append([line.strip()[x] for x in range(len(line.strip()))])
        input_matrix = np.array(input_data)
        dim = (input_matrix.shape[0], input_matrix.shape[1])
        costs = np.full((dim[0], dim[1]), math.inf)
        costs[0, 0] = 0
        potentially_visitable = [(0, (0, 0))]
        heapq.heapify(potentially_visitable)  # start pos with 0 cost
        visited = np.zeros((dim[0], dim[1]))

        def dijkstra():
            def get_neighbours(coord):
                row, col = coord[0], coord[1]
                return [(row+1, col), (row-1, col), (row, col+1), (row, col-1)]

            while len(potentially_visitable) > 0:
                weight, curr_pos = heapq.heappop(potentially_visitable)
                if curr_pos == (dim[0] - 1, dim[1] - 1):
                    break
                visited[curr_pos[0], curr_pos[1]] = 1
                neighbours = get_neighbours(curr_pos)
                for entry in neighbours:
                    if 0 <= entry[0] < dim[0] and 0 <= entry[1] < dim[1] and not visited[entry[0], entry[1]]:
                        cost_entry = int(costs[curr_pos[0], curr_pos[1]]) + int(input_matrix[entry[0], entry[1]])
                        if cost_entry < costs[entry[0], entry[1]]:
                            costs[entry[0], entry[1]] = cost_entry
                            heapq.heappush(potentially_visitable, (cost_entry, (entry[0], entry[1])))

        dijkstra()
        print(costs)
        print("Cost final: " + str(costs[dim[0] - 1, dim[1] - 1]))
