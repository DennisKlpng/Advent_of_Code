#include "utils.hpp"
#include <deque>
#include <functional>
#include <cmath>
#include <numeric>
#include <execution>
#include <mutex>
#include <atomic>

inline int get_digits(int64_t x){
    return  (x < 10 ? 1 :   
            (x < 100 ? 2 :   
            (x < 1000 ? 3 :   
            (x < 10000 ? 4 :   
            (x < 100000 ? 5 :   
            (x < 1000000 ? 6 :   
            (x < 10000000 ? 7 :  
            (x < 100000000 ? 8 :  
            (x < 1000000000 ? 9 : 
            (x < 10000000000 ? 10 :
            (x < 100000000000 ? 11 :
            (x < 1000000000000 ? 12 :
            (x < 10000000000000 ? 13 :   
            14)))))))))))));  
}

static std::vector<int64_t> pows{1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000,
                                 1000000000, 10000000000, 100000000000, 1000000000000, 10000000000000};

bool check_if_valid(int64_t tgt_result, const std::deque<int64_t>& input,
                    int64_t curr_value, int8_t index, bool part_2 = false){
    if (index < 0) return tgt_result == curr_value;
    if (curr_value < tgt_result) return false;
    int64_t num_to_check = input[index];
    int8_t digits = get_digits(num_to_check);
    //int8_t digits = (int)log10(num_to_check) + 1;
    if (part_2 && ((curr_value - num_to_check) % pows[digits]) == 0){ //concat
        if(check_if_valid(tgt_result, input, curr_value/pows[digits], index-1, part_2)) return true;
    }
    if (curr_value % num_to_check == 0){ //multiplication
        if(check_if_valid(tgt_result, input, curr_value/num_to_check, index-1, part_2)) return true;
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
    std::atomic<uint64_t> res_1;
    std::atomic<uint64_t> res_2;
    std::for_each(std::execution::par, vec_data.begin(), vec_data.end(),[&](auto sub_deque){
        int64_t tgt = sub_deque[0];
        sub_deque.pop_front();
        int64_t first_val = sub_deque[0];
        sub_deque.pop_front();
        if(check_if_valid(first_val, sub_deque, tgt, sub_deque.size() - 1, false)){
            res_1 += tgt;
        }
        if(check_if_valid(first_val, sub_deque, tgt, sub_deque.size() - 1, true)){
            res_2 += tgt;
        }
    });
    res.first = res_1;
    res.second = res_2;

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