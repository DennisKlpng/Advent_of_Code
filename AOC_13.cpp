#include "utils.hpp"
#include <cmath>

inline double get_a(const double x, const double b, const double xa, const double xb){
    return(round((x - b*xb)/xa));
}

inline double get_b(const double x, const double y, const double xa, const double ya,
                      const double xb, const double yb){
        double numerator = y - round(x*ya/xa);
        double divisor = yb - round(xb*ya/xa);
        return round(numerator / divisor);
    }



std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};

    auto machines = read_file_as_blocks_lines(filename);
    for(auto& m: machines){
        auto button_a = get_ints_from_string(m.at(0));
        auto button_b = get_ints_from_string(m.at(1));
        auto target = get_ints_from_string(m.at(2));

        auto b = get_b(target[0], target[1], button_a[0], button_a[1], button_b[0], button_b[1]);
        auto a = get_a(target[0], b, button_a[0], button_b[0]);
        if(a >= 0 && a <= 100 && b >= 0 && b <= 100){
            print(a, " ", b, "\n");
            res.first += static_cast<uint64_t>(round(a*3.0 + b));
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