#include "utils.hpp"
#include <numeric>

inline std::vector<uint64_t> split_num(const uint64_t in){
    if(in == 0) return std::vector<uint64_t>{1};
    auto str = std::to_string(in);
    if(str.size() % 2 == 0){
        return std::vector<uint64_t>{std::stoull(str.substr(0, str.size()/2)), std::stoull(str.substr(str.size()/2))};
    }
    return std::vector<uint64_t>{2024*in};
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};
    std::vector<uint64_t> vals = split_string_int<std::vector<uint64_t>>(read_file_as_lines(filename)[0]);
    std::map<uint64_t, uint64_t> map_nums;
    for(auto& i : vals){
        if(!map_nums.try_emplace(i, 1).second) map_nums[i]++;
    }
    for(int i = 0; i < 75; i++){
        std::map<uint64_t, uint64_t> map_nums_tmp;
        for(auto& [k, v]: map_nums){
            auto new_nums = split_num(k);
            for(auto& j : new_nums){
                if(!map_nums_tmp.try_emplace(j, v).second) map_nums_tmp[j] += v;
            }
        }
        map_nums = map_nums_tmp;
        if(i == 24){
            for(auto& [k, v]: map_nums){
                res.first += v;
            }
        }
    }
    for(auto& [k, v]: map_nums){
        res.second += v;
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_11.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_11.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}