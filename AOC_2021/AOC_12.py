import sys
import numpy as np


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        graph = {}
        for line in f:
            graph_edge = line.strip().split("-")
            if graph_edge[0] not in graph.keys():
                graph[graph_edge[0]] = [graph_edge[1]]
            else:
                graph[graph_edge[0]].append(graph_edge[1])

            if graph_edge[1] not in graph.keys():
                graph[graph_edge[1]] = [graph_edge[0]]
            else:
                graph[graph_edge[1]].append(graph_edge[0])

        def depth_first(node, path_dict, double_visit_available):
            if str(node) == "start":  # just bypass 2nd visit to start
                return 0
            num_sub_paths = 0
            follow_dict = path_dict.copy()
            follow_double_visit = double_visit_available
            if node not in path_dict.keys():
                follow_dict[node] = 1
            else:
                follow_dict[node] += 1
            if node == "end":
                return 1
            if str(node).islower() and follow_dict[node] > 1:
                if not double_visit_available:
                    return 0
                else:
                    follow_double_visit = False
            for subnode in graph[node]:
                num_sub_paths += depth_first(subnode, follow_dict, follow_double_visit)
            return num_sub_paths

        num_paths_pt1 = num_paths_pt2 = 0
        for entry in graph["start"]:
            num_paths_pt1 += depth_first(entry, {"start": 1}, False)
            num_paths_pt2 += depth_first(entry, {"start": 1}, True)
        print("Num paths pt 1: " + str(num_paths_pt1))
        print("Num paths pt 2: " + str(num_paths_pt2))
