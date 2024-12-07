#include "utils.hpp"
#include <deque>
#include <functional>
#include <cmath>
#include <numeric>

inline uint64_t add(uint64_t a, uint64_t b){return a + b;};
inline uint64_t mul(uint64_t a, uint64_t b){return a * b;};
inline uint64_t concat(uint64_t a, uint64_t b){
    uint64_t val = b;
    while(b != 0) {b /= 10; a*= 10;}
    return val + a;
}

static const std::vector<std::function<uint64_t(uint64_t, uint64_t)>> funcs_pt1{add, mul};
static const std::vector<std::function<uint64_t(uint64_t, uint64_t)>> funcs_pt2{add, mul, concat};

bool check_if_valid(uint64_t tgt_result, const std::deque<uint64_t>& input,
                    std::vector<std::function<uint64_t(uint64_t, uint64_t)>> ops,
                    uint64_t curr_value, uint8_t index){
    if(input.size() == index) return tgt_result == curr_value;
    if(curr_value > tgt_result) return false;
    if(std::accumulate(input.begin() + index, input.end(), curr_value, concat) < tgt_result) return false;
    for(auto& func: ops){
        uint64_t new_value = func(curr_value, input[index]);
        uint8_t new_index = index + 1;
        if(check_if_valid(tgt_result, input, ops, new_value, new_index)) return true;
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
        uint64_t init_val = data[0];
        data.pop_front();
        if(check_if_valid(tgt, data, funcs_pt1, init_val, 0)) res.first += tgt;
        if(check_if_valid(tgt, data, funcs_pt2, init_val, 0)) res.second += tgt;
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