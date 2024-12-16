#include "utils.hpp"
#include <queue>

//x: right, y: down

typedef enum mp{FREE = 0, WALL = 1} mp;
typedef enum move{UP = 1, RIGHT = 2, DOWN = 3, LEFT = 4 }move;

struct pos{
    pos(int64_t x, int64_t y, move mv): x(x), y(y), mv(mv){};
    pos(){};
    int64_t x;
    int64_t y;
    move mv;

    bool operator <(const pos& p) const{
        return x < p.x || (x == p.x && y < p.y) || (x == p.x && y == p.y && mv < p.mv);
    }
    bool operator == (const pos& p) const{
        return x == p.x && y == p.y && mv == p.mv;
    }
};

typedef std::pair<int64_t, int64_t> pt;

struct map_entry{
    map_entry(mp type): type(type){};
    map_entry(){};
    mp type;
    int64_t curr_cost = std::numeric_limits<int64_t>::max();
    std::vector<pos> pred = {};
};
struct heap_entry{
    pos pos;
    int64_t curr_cost;
};

inline std::vector<pos> get_neighbours(const pos& in){
    const auto& x = in.x;
    const auto& y = in.y;
    return std::vector<pos>{pos(x, y-1, move::UP), pos(x+1, y, move::RIGHT), pos(x, y+1, move::DOWN), pos(x-1, y, move::RIGHT)};
}

std::pair<int64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<int64_t, uint64_t> res{std::numeric_limits<int64_t>::max(), 0};

    auto lines = read_file_as_lines(filename);
    std::map<pos, map_entry> warehouse_map;

    pos start;
    pt end(0,0);
    int64_t y = 0;
    std::vector<pos> end_pos;
    for(const auto& line_map: lines){
        int64_t x = 0;
        for(const auto&c : line_map){
            if(c == '.') {
                warehouse_map.emplace(pos(x, y, move::UP), map_entry(mp::FREE));
                warehouse_map.emplace(pos(x, y, move::RIGHT), map_entry(mp::FREE));
                warehouse_map.emplace(pos(x, y, move::DOWN), map_entry(mp::FREE));
                warehouse_map.emplace(pos(x, y, move::LEFT), map_entry(mp::FREE));
            }
            else if(c == '#') {
                warehouse_map.emplace(pos(x, y, move::UP), map_entry(mp::WALL));
                warehouse_map.emplace(pos(x, y, move::RIGHT), map_entry(mp::WALL));
                warehouse_map.emplace(pos(x, y, move::DOWN), map_entry(mp::WALL));
                warehouse_map.emplace(pos(x, y, move::LEFT), map_entry(mp::WALL));
            }
            else if(c == 'S') {
                start = pos(x, y, move::RIGHT);
            }
            else if(c == 'E') {
                end = pt(x, y);
                warehouse_map.emplace(pos(x, y, move::UP), map_entry(mp::FREE));
                warehouse_map.emplace(pos(x, y, move::RIGHT), map_entry(mp::FREE));
                warehouse_map.emplace(pos(x, y, move::DOWN), map_entry(mp::FREE));
                warehouse_map.emplace(pos(x, y, move::LEFT), map_entry(mp::FREE));
            }
            x++;
        }
        y++;
    }

    //Part 1: dijkstra (each pos for each orientation is a separate node)
    auto custom_comp = [](heap_entry& l, heap_entry& r){
        return l.curr_cost > r.curr_cost;
    };
    std::priority_queue<heap_entry, std::vector<heap_entry>, decltype(custom_comp)> node_heap;
    node_heap.push(heap_entry(start, 0));
    
    while(!node_heap.empty()){
        auto curr_node = node_heap.top();
        node_heap.pop();
        auto& curr_cost = curr_node.curr_cost;
        if(curr_node.pos.x == end.first && curr_node.pos.y == end.second){
            if(curr_cost < res.first) {
                res.first = curr_cost;
                end_pos.push_back(curr_node.pos);
            }
        };

        //get neighbours
        auto nbs = get_neighbours(curr_node.pos);
        for(auto& nb : nbs){
            //calc cost to move there
            if(warehouse_map[nb].type == mp::WALL) continue;
            if(abs(nb.mv - curr_node.pos.mv) == 2) continue; //trying to backtrack: doesn't make sense
            int64_t cost_move = 1;
            if(nb.mv != curr_node.pos.mv) cost_move += 1000; //we have to turn
            auto& map_node = warehouse_map[nb];
            if(map_node.curr_cost > curr_cost + cost_move){
                map_node.pred.clear();
                map_node.pred.push_back(curr_node.pos);
                map_node.curr_cost = curr_cost + cost_move;
                node_heap.push(heap_entry(nb, curr_cost + cost_move));
            }
            else if(map_node.curr_cost == curr_cost + cost_move){
                map_node.pred.push_back(curr_node.pos);
            }
        }
    }

    //Part 2: back track from the end
    std::set<pt> spots{end};
    for(auto& end_start: end_pos){
        std::deque<pos> queue{end_start};
        while(!queue.empty()){
            auto elem = queue.back();
            if(elem == start) break;
            for(auto& pred: warehouse_map[elem].pred){
                if(std::find(queue.begin(), queue.end(), pred) != queue.end()) continue;
                queue.push_front(pred);
                spots.emplace(pt(pred.x, pred.y));
            }
            queue.pop_back();
        }
    }
    res.second = spots.size();

    return res;
}

int main(){
    std::pair<int64_t, uint64_t> res = solve_puzzle("inputs/Test_16_2.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_16.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}