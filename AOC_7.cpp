#include "utils.hpp"
#include <deque>
#include <functional>
#include <cmath>
#include <numeric>
#include <execution>
#include <mutex>

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

bool check_if_valid(int64_t tgt_result, const std::deque<int64_t>& input,
                    int64_t curr_value, int8_t index, bool part_2 = false){
    // if(input.size() == index) return tgt_result == curr_value;
    // if(curr_value > tgt_result) return false;
    // //try concatenating everything that's left, if it's smaller than the tgt => can't be reachead anymore
    // if(std::accumulate(input.begin() + index, input.end(), curr_value, concat) < tgt_result) return false;
    // for(auto& func: ops){
    //     uint64_t new_value = func(curr_value, input[index]);
    //     uint8_t new_index = index + 1;
    //     if(check_if_valid(tgt_result, input, ops, new_value, new_index)) return true;
    // }
    if (index < 0) return tgt_result == curr_value;
    if (curr_value < tgt_result) return false;
    int64_t num_to_check = input[index];
    if (curr_value % num_to_check == 0){ //multiplication
        if(check_if_valid(tgt_result, input, curr_value/num_to_check, index-1, part_2)) return true;
    }
    int8_t digits = (int)log10(num_to_check) + 1;
    if (part_2 && ((curr_value - num_to_check) % (int64_t)pow(10, digits)) == 0){
        if(check_if_valid(tgt_result, input, curr_value/pow(10, digits), index-1, part_2)) return true;
    }
    if(check_if_valid(tgt_result, input, curr_value-num_to_check, index-1, part_2)) return true;


    return false;
}


std::pair<int64_t, int64_t> solve_puzzle(std::string filename){
    std::pair<int64_t, int64_t> res{0, 0};
    std::vector<std::string> lines = read_file_as_lines(filename);
    std::vector<std::deque<int64_t>> vec_data;
    for(auto& line: lines){
        std::deque<int64_t> data = split_string_int<std::deque<int64_t>>(line, ' ', ":");
        vec_data.push_back(data);
    }
    std::mutex num_mutex;
    std::for_each(std::execution::par, vec_data.begin(), vec_data.end(),[&](auto sub_deque){
        int64_t tgt = sub_deque[0];
        sub_deque.pop_front();
        int64_t first_val = sub_deque[0];
        sub_deque.pop_front();
        if(check_if_valid(first_val, sub_deque, tgt, sub_deque.size() - 1, false)){
            num_mutex.lock();
            res.first += tgt;
            num_mutex.unlock();
        }
        if(check_if_valid(first_val, sub_deque, tgt, sub_deque.size() - 1, true)){
            num_mutex.lock();
            res.second += tgt;
            num_mutex.unlock();
        }
    });

    return res;
}

int main(){
    std::pair<int64_t, int64_t> res = solve_puzzle("inputs/Test_7.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_7.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}