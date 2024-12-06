#include "utils.hpp"

std::pair<int, int> solve_puzzle(std::string filename){
    std::pair<int, int> res{0, 0};
    std::vector<std::string> vec_input = read_file_as_lines(filename);

    return res;
}

int main(){
    std::pair<int, int> res = solve_puzzle("Test_6.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");

    return 0;

    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "Data_6.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}