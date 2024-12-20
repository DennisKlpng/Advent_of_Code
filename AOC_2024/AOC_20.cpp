#include "utils.hpp"
#include "queue"

typedef std::pair<int16_t, int16_t> pt;
inline std::vector<pt> get_neighbours(pt in){
    const auto& x = in.first;
    const auto& y = in.second;
    return std::vector<pt>{pt(x-1, y), pt(x+1, y), pt(x, y-1), pt(x, y+1)};
}

constexpr char get_elem(const int64_t obj){
    if(obj == 0) return '.';
    if(obj == 1) return '#';
    return '@';
}

static int64_t max_x = 0;
static int64_t max_y = 0;

void visualize_map(const std::map<pt, int64_t>& curr_map){
    std::string empt_line(max_x, ' ');
    std::vector<std::string> vec_lines(max_y, empt_line);

    for(const auto& [k, v]: curr_map){
        vec_lines[k.second][k.first] = get_elem(v);
    }
    for(const auto& line: vec_lines){
        print(line);
    }
    print("");
}

 std::vector<pt> get_path(const pt& start, const pt& goal, std::map<pt, pt>& map_world){
    std::vector<pt> path{start};
    pt curr_pos = start;
    std::int64_t cost_to_reach = 0;
    while(curr_pos != goal){
        cost_to_reach++;
        for(auto& nb: get_neighbours(curr_pos)){
            if(map_world[nb].first == 1) continue;
            if(std::find(path.begin(), path.end(), nb) != path.end()) continue;
            path.push_back(nb);
            map_world[nb].second = cost_to_reach;
            curr_pos = nb;
        }
    }
    path.push_back(goal);
    return path;
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename, bool test=true){
    std::pair<uint64_t, uint64_t> res{0, 0};
    auto lines = read_file_as_lines(filename);
    pt start, end;

    //0 free, 1 occupied
    std::map<pt, pt> map_world;
    max_x = lines[0].size();
    max_y = lines.size();

    uint64_t  y = 0;
    for(auto const& line: lines){
        uint64_t  x = 0;
        for(auto const& c : line){
            if(c == '.') {
                map_world.try_emplace(pt(x, y), pt(0, 0));
            }
            if(c == '#') {
                map_world.try_emplace(pt(x, y), pt(1, 0));
            }
            if(c == 'S'){
                start = pt(x, y);
                map_world.try_emplace(pt(x, y), pt(0, 0));
            }
            if(c == 'E'){
                map_world.try_emplace(pt(x, y), pt(0, 0));
                end = pt(x, y);
            }
            x++;
        }
        y++;
    }
    std::vector<pt> path_tgt = get_path(start, end, map_world);
    struct cheat{
        pt start_warp;
        pt end_warp;
        uint64_t saving;
        bool operator <(const cheat& p) const{
            if(start_warp.first < p.start_warp.first) return true;
            if(start_warp.second < p.start_warp.second) return true;
            if(end_warp.first < p.end_warp.first) return true;
            if(end_warp.second < p.end_warp.second) return true;
            return false;
        }
        bool operator == (const cheat& p) const{
            return start_warp == p.start_warp && end_warp == p.end_warp;
        }
    };
    // int64_t cost_end = map_world[end].second;
    std::set<cheat> cheats_1{};
    std::set<cheat> cheats_2{};

    int64_t threshold = (test)? 50 : 100;

    //we know that the diff between first and second in path must be at least 100 for the real maze (50 for part 2 test)
    for(uint64_t i = 0; i < path_tgt.size(); i++){
        auto pos = path_tgt[i];
        if(i + threshold >= path_tgt.size()) continue;
        for(uint64_t j = i; j < path_tgt.size(); j++){
            auto pos_tgt = path_tgt[j];
            int64_t manhattan_dist = abs(pos_tgt.first - pos.first) + abs(pos_tgt.second - pos.second);
            int64_t cost_diff = j - i - manhattan_dist;
            if(cost_diff < threshold) continue;
            if(manhattan_dist <= 2) cheats_1.emplace(cheat(pos, pos_tgt, cost_diff));
            if(manhattan_dist <= 20) cheats_2.emplace(cheat(pos, pos_tgt, cost_diff));
        }
    }
 
    res.first = cheats_1.size();
    res.second = cheats_2.size();

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_20.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    bool test = false;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_20.txt", test);
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}