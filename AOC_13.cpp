#include "utils.hpp"
#include <cmath>

inline int64_t get_a(const int64_t x, const int64_t b, const int64_t xa, const int64_t xb){
    if((x-b*xb)%xa != 0) return 0;
    return((x - b*xb)/xa);
}

inline int64_t get_b(const int64_t x, const int64_t y, const int64_t xa, const int64_t ya,
                      const int64_t xb, const int64_t yb){
    int64_t numerator = y*xa - x*ya;
    int64_t divisor = yb*xa - xb*ya;
    if(numerator % divisor != 0) return 0;
    return numerator / divisor;
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};

    auto machines = read_file_as_blocks_lines(filename);
    int i = 0;
    for(auto& m: machines){
        i++;
        auto button_a = get_ints_from_string(m.at(0));
        auto button_b = get_ints_from_string(m.at(1));
        auto target = get_ints_from_string(m.at(2));
        {//pt1
            auto b = get_b(target[0], target[1], button_a[0], button_a[1], button_b[0], button_b[1]);
            auto a = get_a(target[0], b, button_a[0], button_b[0]);
            if(a >= 0 && b >= 0){
                if((a * button_a[0] + b* button_b[0] == target[0]) &&
                   (a * button_a[1] + b* button_b[1] == target[1])){
                    res.first += (a*3 + b);
                }
            }
        } 
        {//pt2
            target[0] += (int64_t)10000000000000;
            target[1] += (int64_t)10000000000000;
            auto b = get_b(target[0], target[1], button_a[0], button_a[1], button_b[0], button_b[1]);
            auto a = get_a(target[0], b, button_a[0], button_b[0]);
            if(a >= 0 && b >= 0){
                if(a * button_a[0] + b* button_b[0] != target[0]) continue;
                if(a * button_a[1] + b* button_b[1] != target[1]) continue;
                res.second += (a * 3 + b);
            }
        }
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_13.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_13.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}