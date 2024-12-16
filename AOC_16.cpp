#include "utils.hpp"
#include <queue>

typedef enum mp{FREE = 0, WALL = 1}mp;
typedef enum move{UP = 1, RIGHT = 2, DOWN = 3, LEFT = 4}move;
//x: right, y: down
typedef std::pair<int64_t, int64_t> pt;

struct map_entry{
    mp type;
    int64_t curr_cost;
    move move_reached;
};
struct heap_entry{
    pt pos;
    int64_t curr_cost;
    move move_reached;
};

inline std::vector<pt> get_neighbours(const pt& in){
    const auto& x = in.first;
    const auto& y = in.second;
    return std::vector<pt>{pt(x, y-1), pt(x+1, y), pt(x, y+1), pt(x-1, y)};
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};

    auto lines = read_file_as_lines(filename);
    // int64_t max_x = lines.at(0).size() - 1;
    // int64_t max_y = lines.size() - 1;
    //tuple: type of "node", cost to reach that node, move direction how it was reached
    std::map<pt, map_entry> warehouse_map;

    pt start;
    pt end;
    int64_t y = 0;
    int64_t max = std::numeric_limits<int64_t>::max();
    for(const auto& line_map: lines){
        int64_t x = 0;
        for(const auto&c : line_map){
            if(c == '.') {
                warehouse_map.try_emplace(pt(x, y), map_entry(mp::FREE, max, static_cast<move>(0)));
            }
            else if(c == '#') {
                warehouse_map.try_emplace(pt(x, y), map_entry(mp::WALL, max, static_cast<move>(0)));
            }
            else if(c == 'S') {
                start = pt(x, y);
            }
            else if(c == 'E') {
                end = pt(x, y);
                warehouse_map.try_emplace(pt(x, y), map_entry(mp::FREE, max, static_cast<move>(0)));
            }
            x++;
        }
        y++;
    }

    auto custom_comp = [](heap_entry& l, heap_entry& r){
        return l.curr_cost > r.curr_cost;
    };
    std::priority_queue<heap_entry, std::vector<heap_entry>, decltype(custom_comp)> node_heap;
    node_heap.push(heap_entry(start, 0, move::RIGHT));
    

    //dijkstra
    while(!node_heap.empty()){
        auto curr_node = node_heap.top();
        node_heap.pop();
        if(curr_node.pos == end) break;
        auto& curr_cost = curr_node.curr_cost;

        //get neighbours
        auto nbs = get_neighbours(curr_node.pos);
        int8_t nb_index = 0;
        for(auto& nb : nbs){ //order: same as move=> up, right, down, left
            nb_index++;
            //calc cost to move there
            if(warehouse_map[nb].type == mp::WALL) continue;
            if(abs(nb_index - curr_node.move_reached) == 2) continue; //trying to backtrack: doesn't make sense
            int64_t cost_move = 1;
            if(nb_index != curr_node.move_reached) cost_move += 1000; //we have to turn
            auto& map_node = warehouse_map[nb];
            if(map_node.curr_cost > curr_cost + cost_move){
                map_node.curr_cost = curr_cost + cost_move;
                map_node.move_reached = static_cast<move>(nb_index);
                node_heap.push(heap_entry(nb, curr_cost + cost_move, static_cast<move>(nb_index)));
            }
        }
    }
    res.first = warehouse_map[end].curr_cost;
    res.second = 0;

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_16_2.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_16.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}