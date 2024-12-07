#include "utils.hpp"
#include <deque>
#include <functional>
#include <cmath>
#include <numeric>
#include <execution>

inline uint64_t add(uint64_t a, uint64_t b){return a + b;};
inline uint64_t mul(uint64_t a, uint64_t b){return a * b;};
inline uint64_t concat(uint64_t a, uint64_t b){
    if (b < 10) return 10*a + b;
    if (b < 100) return 100*a + b;
    if (b < 1000) return 1000*a + b;
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
    //try concatenating everything that's left, if it's smaller than the tgt => can't be reachead anymore
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
    std::vector<std::deque<uint64_t>> vec_data;
    for(auto& line: lines){
        std::deque<uint64_t> data = split_string_int<std::deque<uint64_t>>(line, ' ', ":");
        vec_data.push_back(data);
    }
    std::mutex num_mutex;
    std::for_each(std::execution::par, vec_data.begin(), vec_data.end(),[&](auto sub_deque){
        uint64_t tgt = sub_deque[0];
        sub_deque.pop_front();
        uint64_t init_val = sub_deque[0];
        sub_deque.pop_front();
        if(check_if_valid(tgt, sub_deque, funcs_pt1, init_val, 0)){
            num_mutex.lock();
            res.first += tgt;
            num_mutex.unlock();
        }
        if(check_if_valid(tgt, sub_deque, funcs_pt2, init_val, 0)){
            num_mutex.lock();
            res.second += tgt;
            num_mutex.unlock();
        }
    });

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