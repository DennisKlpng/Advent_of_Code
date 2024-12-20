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
        costs = np.full((5*dim[0], 5*dim[1]), math.inf)
        costs[0, 0] = 0
        potentially_visitable = [(0, (0, 0))]
        heapq.heapify(potentially_visitable)  # start pos with 0 cost
        visited = np.zeros((5*dim[0], 5*dim[1]))

        def dijkstra():
            def get_neighbours(coord):
                row, col = coord[0], coord[1]
                return [(row+1, col), (row-1, col), (row, col+1), (row, col-1)]

            def get_tile_cost(curr_coord, tgt_coord):
                row_c, col_c = curr_coord[0], curr_coord[1]
                row_t, col_t = tgt_coord[0], tgt_coord[1]

                cost_c = costs[row_c, col_c]
                cost_t = int(input_matrix[row_t % dim[0], col_t % dim[1]]) + int(row_t / dim[0]) + int(col_t / dim[1])
                cost_t = cost_t if cost_t < 10 else cost_t % 9
                cost = cost_c + cost_t

                return cost

            found_sol_pt1, found_sol_pt2 = False, False
            while len(potentially_visitable) > 0:
                weight, curr_pos = heapq.heappop(potentially_visitable)
                if curr_pos == (dim[0] - 1, dim[1] - 1):
                    found_sol_pt1 = True
                elif curr_pos == (5*dim[0] - 1, 5*dim[1] - 1):
                    found_sol_pt2 = True
                if found_sol_pt1 and found_sol_pt2:
                    break
                visited[curr_pos[0], curr_pos[1]] = 1
                neighbours = get_neighbours(curr_pos)
                for entry in neighbours:
                    if 0 <= entry[0] < 5*dim[0] and 0 <= entry[1] < 5*dim[1] and not visited[entry[0], entry[1]]:
                        cost_entry = get_tile_cost(curr_pos, entry)
                        if cost_entry < costs[entry[0], entry[1]]:
                            costs[entry[0], entry[1]] = cost_entry
                            heapq.heappush(potentially_visitable, (cost_entry, (entry[0], entry[1])))

        dijkstra()

        print("Cost final part 1: " + str(costs[dim[0] - 1, dim[1] - 1]))
        print("Cost final part 2: " + str(costs[5*dim[0] - 1, 5*dim[1] - 1]))
