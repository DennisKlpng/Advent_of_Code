#include "utils.hpp"

typedef std::pair<uint16_t, uint16_t> pt;
constexpr pt operator+(const pt& pt1, const pt& pt2){
    return pt(pt1.first + pt2.first, pt1.second + pt2.second);
}
constexpr pt operator*(const int16_t& scale, const pt& pt2){
    return pt(scale*pt2.first, scale*pt2.second);
}

std::vector<pt> get_diag_neighbours(){
    return std::vector<pt>{pt(-1, -1), pt(-1, 0), pt(-1, 1), pt(0, -1), pt(0, 1), pt(1, -1), pt(1, 0), pt(1, 1)};
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};

    auto data = read_file_as_lines(filename);
    int16_t max_y = data.size() - 1;
    int16_t max_x = data[0].size() - 1;

    //pad 3 on each side
    std::map<pt, int16_t> map_world;
    int16_t ord_x = 'X';
    int16_t ord_m = 'M';
    int16_t ord_a = 'A';
    int16_t ord_s = 'S';

    std::vector<pt> s_locs;
    std::set<pt> a_in_mas_locs;
    for(int16_t x = 0; x <= max_x + 6; x++){
        for(int16_t y = 0; y <= max_y + 6; y++){
            if (x < 3 || x > max_x + 3 || y < 3 || y > max_y + 3){
                map_world.try_emplace(pt(x, y), 0);
            }
            else{
                map_world.try_emplace(pt(x, y), 0);
            }
        }
    }

    int64_t y = 3;
    for(const auto& line: data){
        int64_t x = 3;
        for(const auto&c : line){
            int16_t val = c;
            map_world[pt(x, y)] = val;
            if(c == 'S') s_locs.push_back(pt(x, y));
            x++;
        }
        y++;
    }
    for(auto&s : s_locs){
        for(auto& nb_vec: get_diag_neighbours()){
            auto pt_a = s + nb_vec;
            auto pt_m = s + 2*nb_vec;
            auto pt_x = s + 3*nb_vec;
            if(map_world[pt_a] != ord_a) continue;
            if(map_world[pt_m] != ord_m) continue;
            //save a locations where A and M are on a diagonal (and therefore by design also S)
            if(pt_a.first != pt_m.first && pt_a.second != pt_m.second) a_in_mas_locs.emplace(pt_a);
            if(map_world[pt_x] != ord_x) continue;
            res.first++;
        }
    }
    //pt2: just check that both diagonals involving an "A" are "M" + "S"
    for(auto& a_cand : a_in_mas_locs){
        int16_t ord_diag_1 = map_world[pt(a_cand.first - 1, a_cand.second + 1)] 
                           + map_world[pt(a_cand.first + 1, a_cand.second - 1)];
        int16_t ord_diag_2 = map_world[pt(a_cand.first + 1, a_cand.second + 1)] 
                           + map_world[pt(a_cand.first - 1, a_cand.second - 1)];
        if(ord_diag_1 == (ord_m + ord_s) && ord_diag_2 == (ord_m + ord_s)) res.second++;
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_4.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_4.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}