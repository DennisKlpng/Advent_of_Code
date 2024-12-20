#include "utils.hpp"
#include <deque>

inline std::vector<std::pair<int16_t, int16_t>> get_neighbours(std::pair<int16_t, int16_t> in){
    const auto& x = in.first;
    const auto& y = in.second;
    typedef std::pair<int16_t, int16_t> pt;
    return std::vector<pt>{pt(x-1, y), pt(x+1, y), pt(x, y-1), pt(x, y+1)};
}

uint64_t bfs_trail(std::pair<int16_t, int16_t> start_position, 
                  std::map<std::pair<int16_t, int16_t>, uint8_t>& map_world, const bool pt1 = true){
    uint64_t score = 0;
    std::deque<std::pair<int16_t, int16_t>> visited = {start_position};
    std::deque<std::pair<int16_t, int16_t>>::iterator pos = visited.begin();
    while(pos != visited.end()){
        uint8_t curr_height = map_world[*pos];
        //get 4 direct neighbours
        for(auto const& nb : get_neighbours(*pos)){
            if(map_world[nb] == curr_height + 1){
                if(pt1){
                    if(std::find(visited.begin(), visited.end(), nb) == visited.end()){
                        visited.push_back(nb);
                        if (curr_height + 1 == 9) score++;
                    }
                }
                else{
                    visited.push_back(nb);
                    if (curr_height + 1 == 9) score++;
                }
            }
        }
        pos++;
    }  
    return score;
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};
    std::vector<std::string> data = read_file_as_lines(filename);

    std::map<std::pair<int16_t, int16_t>, uint8_t> map_world;
    std::vector<std::pair<int16_t, int16_t>> trailheads;
    for(uint64_t x = 0; x < data[0].size() + 2; x++){
        for(uint64_t y = 0; y < data.size() + 2; y++){
            if ((x == 0 || x == data[0].size() + 1) && (y == 0 && y == data.size() + 1)){
                map_world.try_emplace(std::pair<int16_t, int16_t>(x, y), 0);
            }
        }
    }
    int16_t y = 1;
    for(auto const& line: data){
        int16_t x = 1;
        for(auto const& c : line){
            uint8_t height = std::stoi(std::string(1, c));
            map_world.try_emplace(std::pair<int16_t, int16_t>(x, y), height);
            if(height == 0) trailheads.push_back(std::pair<int16_t, int16_t>(x, y));
            x++;
        }
        y++;
    }

    for(auto const& start_pos : trailheads){
        res.first += bfs_trail(start_pos, map_world);
        res.second += bfs_trail(start_pos, map_world, false);
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_10.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_10.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}