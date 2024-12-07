#include "utils.hpp"
#include <deque>
#include <functional>

inline uint64_t add(uint64_t a, uint64_t b){return a + b;};
inline uint64_t mul(uint64_t a, uint64_t b){return a * b;};
inline uint64_t concat(uint64_t a, uint64_t b){return stoll(std::to_string(a) + std::to_string(b));}

static const std::vector<std::function<uint64_t(uint64_t, uint64_t)>> funcs_pt1{add, mul};
static const std::vector<std::function<uint64_t(uint64_t, uint64_t)>> funcs_pt2{add, mul, concat};

bool check_if_valid(uint64_t tgt_result, std::deque<uint64_t> input,
                    std::vector<std::function<uint64_t(uint64_t, uint64_t)>> ops){
    if(input.size() == 1) return tgt_result == input[0];
    for(auto& func: ops){
        std::deque<uint64_t> in_loc = input;
        in_loc[1] = func(in_loc[0], in_loc[1]);
        in_loc.pop_front();
        if(check_if_valid(tgt_result, in_loc, ops)) return true;
    }

    return false;
}


std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};
    std::vector<std::string> lines = read_file_as_lines(filename);
    for(auto& line: lines){
        std::deque<uint64_t> data = split_string_int<std::deque<uint64_t>>(line, ' ', ":");
        uint64_t tgt = data[0];
        data.pop_front();
        if(check_if_valid(tgt, data, funcs_pt1)) res.first += tgt;
        if(check_if_valid(tgt, data, funcs_pt2)) res.second += tgt;
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_7.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_7.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}