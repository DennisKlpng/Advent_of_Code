#include "utils.hpp"
#include <deque>

typedef std::pair<int16_t, int16_t> pt;
typedef std::tuple<int16_t, int16_t, int16_t> dir_pt;

inline int16_t get_dir(const pt& in, const pt& in_2){
    if(in_2.second < in.second) return 1;
    if(in_2.second > in.second) return 3;
    if(in_2.first < in.first) return 4;
    if(in_2.first < in.first) return 2;
    return 0;
}

inline std::vector<pt> get_neighbours(pt in){
    const auto& x = in.first;
    const auto& y = in.second;
    return std::vector<pt>{pt(x-1, y), pt(x+1, y), pt(x, y-1), pt(x, y+1)};
}

uint64_t calc_field_score(const pt& top_left, const std::map<pt, char>& map_world, std::set<pt>& visited_set, bool pt2 = false){
    std::deque<pt> visited = {top_left};
    //not coordinates, but <dir, coordinate that doesn't change when moving>
    std::map<pt, std::vector<int16_t>> fence_pieces;
    std::deque<pt>::iterator pos = visited.begin();
    uint64_t fences = 0;
    while(pos != visited.end()){
        for(auto const& nb : get_neighbours(*pos)){
            if(map_world.at(nb) == map_world.at(*pos)){ //still in the area
                if(std::find(visited.begin(), visited.end(), nb) == visited.end()){
                    //didn't visit before => nbs still to analyze
                    visited.push_back(nb);
                }
            }
            else{
                //if dir = 1 (up) or 3 (down) => y is the same, if 2 (right) or 4 (left) x is the same
                auto dir = get_dir(*pos, nb);
                int16_t parallel_to_border;
                int16_t orth_to_border;
                if(dir == 1 || dir == 3){
                    orth_to_border = pos->second;
                    parallel_to_border = pos->first;
                }
                else{
                    orth_to_border = pos->first;
                    parallel_to_border = pos->second;
                }
                if(!fence_pieces.try_emplace(pt(dir, orth_to_border), std::vector<int16_t>{parallel_to_border}).second){
                    fence_pieces.at(pt(dir, orth_to_border)).push_back(parallel_to_border);
                }
            }
        }
        pos++;
    }
    for(auto& pos: visited){
        visited_set.emplace(pos);
    }
    for(auto& [k, v]: fence_pieces){
        //we have to check how many fence pieces we have. for pt 1 easy, take every single one
        if(!pt2) fences += v.size();
        else{ //else check how many contiguous pieces we have => sort by coordinate along the fence
            std::sort(v.begin(), v.end());
            //the number of pieces is the number of gaps in the fence + 1. A gap is when dist > 1
            int16_t x_former = -1; //ensures that the first piece is always counter since x starts at 1
            for(auto& n: v){
                if(n > (x_former + 1)){
                    fences++;
                }
                x_former = n;
            }
        }
    }
    

    // print(std::string(&map_world.at(top_left)), " size: ", visited.size(), " fences: ", fences, "\n");

    return fences * visited.size();
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};
    auto data = read_file_as_lines(filename);
    std::map<pt, char> map_world;
    for(uint64_t x = 0; x < data[0].size() + 2; x++){
        for(uint64_t y = 0; y < data.size() + 2; y++){
            if (x == 0 || x == data[0].size() + 1 || y == 0 || y == data.size() + 1){
                std::string border("#");
                map_world.try_emplace(pt(x, y), border[0]);
            }
        }
    }

    std::vector<pt> vec_area_topleft;
    int16_t y = 1;
    for(auto const& line: data){
        int16_t x = 1;
        for(auto const& c : line){
            map_world.try_emplace(pt(x, y), c);
            if(map_world.at(pt(x - 1, y)) != c && map_world.at(pt(x, y - 1)) != c){
                vec_area_topleft.push_back(pt(x, y));
            }
            x++;
        }
        y++;
    }
    
    //map
    std::map<char, std::vector<std::set<pt>>> map_areas; 
    //bfs on all topleft positions
    for(auto& start : vec_area_topleft){
        auto& plant = map_world.at(start);
        bool plant_known = map_areas.contains(plant);
        if(plant_known){
            //there are cases where the start_pos (with the algorith used to determine them) is already in the field
            //of another start_pos => check if that's the case
            bool plant_pos_already_checked = false;
            for(auto& area : map_areas.at(plant)){
                if(area.contains(start)) plant_pos_already_checked = true;
            }
            if(plant_pos_already_checked) continue;
        }
        std::set<pt> pts_new_field;
        res.first += calc_field_score(start, map_world, pts_new_field);
        res.second += calc_field_score(start, map_world, pts_new_field, true);
        if(plant_known){
            map_areas[plant].push_back(pts_new_field);
        }
        else{
            map_areas.emplace(plant, std::vector<std::set<pt>>{pts_new_field});
        }
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_12.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");
    double time_spent;
    // return 0;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_12.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}