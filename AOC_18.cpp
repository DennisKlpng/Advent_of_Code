#include "utils.hpp"
#include <queue>

static uint64_t max_xy = 0;
uint64_t max = std::numeric_limits<uint64_t>::max();


typedef std::pair<uint64_t, uint64_t> pt;
inline std::vector<pt> get_neighbours(pt in){
    const auto& x = in.first;
    const auto& y = in.second;
    return std::vector<pt>{pt(x-1, y), pt(x+1, y), pt(x, y-1), pt(x, y+1)};
}

struct heap_entry{
    pt pos;
    uint64_t curr_cost;
};

constexpr char get_elem(const uint64_t obj){
    if(obj == 0) return '.';
    if(obj == 1) return '#';
    return '@';
}

void visualize_map(const std::map<pt, uint64_t>& curr_map){
    std::string empt_line(max_xy + 1, ' ');
    std::vector<std::string> vec_lines(max_xy + 1, empt_line);

    for(const auto& [k, v]: curr_map){
        vec_lines[k.second][k.first] = get_elem(v);
    }
    for(const auto& line: vec_lines){
        print(line);
    }
    print("");
}

uint64_t dijkstra(const pt& start, const pt& goal, std::map<pt, pt>& map_world){
    auto custom_comp = [](heap_entry& l, heap_entry& r){
        return l.curr_cost > r.curr_cost;
    };
    std::priority_queue<heap_entry, std::vector<heap_entry>, decltype(custom_comp)> node_heap;
    node_heap.push(heap_entry(start, 0));
    
    while(!node_heap.empty()){
        auto curr_node = node_heap.top();
        node_heap.pop();
        auto& curr_cost = curr_node.curr_cost;
        if(curr_node.pos == goal){
            break;
        };

        //get neighbours
        auto nbs = get_neighbours(curr_node.pos);
        for(auto& nb : nbs){
            if(map_world[nb].first == 1) continue;
            if(curr_cost + 1 < map_world[nb].second){
                map_world[nb].second = curr_cost + 1;
                node_heap.push(heap_entry(nb, curr_cost + 1));
            }
        }
    }
    return map_world[goal].second;
}

std::pair<uint64_t, std::string> solve_puzzle(std::string filename, bool test=false){
    std::pair<uint64_t, std::string> res{0, ""};

    auto data = read_file_as_lines(filename);
    max_xy = (test) ? 8 : 72; 
    // 1: occupied, 0: free
    std::map<pt, pt> map_world;

    for(uint64_t x = 0; x <= max_xy; x++){
        for(uint64_t y = 0; y <= max_xy; y++){
            if (x == 0 || x == max_xy || y == 0 || y == max_xy){
                map_world.try_emplace(pt(x, y), pt(1, max));
            }
            else{
                map_world.try_emplace(pt(x, y), pt(0, max));
            }
        }
    }

    uint64_t read_bytes = 0;
    uint64_t read_bytes_max = (test) ? 12 : 1024;
    for(;read_bytes < read_bytes_max; read_bytes++){
        auto coords_line = get_ints_from_string(data[read_bytes]);
        map_world[pt(coords_line[0] + 1, coords_line[1] + 1)].first = 1;
    }

    pt start{1, 1};
    pt goal = (test) ? pt(7, 7) : pt(71, 71);

    res.first = dijkstra(start, goal, map_world);

    return res;
}

int main(){
    std::pair<uint64_t, std::string> res = solve_puzzle("inputs/Test_18.txt", true);
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    bool test = false;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_18.txt", test);
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}