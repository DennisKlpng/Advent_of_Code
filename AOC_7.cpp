#include "utils.hpp"
#include <deque>
#include <functional>
#include <cmath>
#include <numeric>
#include <execution>
#include <mutex>

bool check_if_valid(int64_t tgt_result, const std::deque<int64_t>& input,
                    int64_t curr_value, int8_t index, bool part_2 = false){
    if (index < 0) return tgt_result == curr_value;
    if (curr_value < tgt_result) return false;
    int64_t num_to_check = input[index];
    if (curr_value % num_to_check == 0){ //multiplication
        if(check_if_valid(tgt_result, input, curr_value/num_to_check, index-1, part_2)) return true;
    }
    int8_t digits = (int)log10(num_to_check) + 1;
    if (part_2 && ((curr_value - num_to_check) % (int64_t)pow(10, digits)) == 0){ //concat
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