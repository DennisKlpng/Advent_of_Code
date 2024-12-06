#include "utils.hpp"


std::pair<int, int> solve_puzzle(std::string filename){
    std::pair<int, int> res{0, 0};

    return res;
}

int main(){
    std::pair<int, int> res = solve_puzzle("inputs/Test_7.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_7.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}