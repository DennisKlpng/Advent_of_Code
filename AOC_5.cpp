#include "utils.hpp"

static std::vector<std::pair<int, int>> rule_list;

//return false if they are out of order, true if they are in order (or there is no rule)
auto compare(const int fst, const int snd){
    if(std::find(rule_list.cbegin(), rule_list.cend(), std::pair(snd, fst)) != rule_list.end()){
        return false;
    }
    return true;
}

std::pair<int, int> solve_puzzle(std::string filename){
    std::pair<int, int> res{0, 0};
    rule_list.clear();
    std::vector<std::vector<std::string>> blocks = read_file_as_blocks_lines(filename);
    for (auto& line: blocks[0]){
        std::vector<int> curr = split_string_int(line, '|');
        rule_list.push_back(std::pair(curr[0], curr[1]));
    }
    for (auto& line: blocks[1]){
        std::vector<int> curr = split_string_int(line, ',');
        if(std::is_sorted(curr.cbegin(), curr.cend(), compare)){
            res.first += curr.at((int)(curr.size()/2));
        }
        else{
            std::sort(curr.begin(), curr.end(), compare);
            res.second += curr.at((int)(curr.size()/2));
        }
    }

    return res;
}

int main(){
    std::pair<int, int> res = solve_puzzle("Test_5.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");

    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "Data_5.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}