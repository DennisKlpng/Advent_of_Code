#include "utils.hpp"
#include <numeric>
#include <regex>

int get_muls(std::string& str){
    int ret = 0;
    std::string::const_iterator curr_start(str.cbegin());
    std::smatch match;
    const std::regex reg("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
    while(std::regex_search(curr_start, str.cend(), match, reg)){
        ret += std::stoi(match[1].str()) * std::stoi(match[2].str());
        curr_start = match.suffix().first;
    }
    return ret;
}

std::pair<int, int> solve_puzzle(std::string filename){
    std::pair<int, int> res{0, 0};
    std::vector<std::string> lines = read_file_as_lines(filename);
    std::string text = std::accumulate(lines.begin(), lines.end(), std::string(""));
    res.first = get_muls(text);
    std::regex repl("don't\\(\\).*?(do\\(\\)|$)");
    text = std::regex_replace(text, repl, "");
    res.second = get_muls(text);

    return res;
}

int main(){
    std::pair<int, int> res = solve_puzzle("Test_3.txt");
    std::cout << "TEST res pt 1: " << res.first << " pt 2: " << res.second << "\n";
    
    res = profile_function(solve_puzzle, "Data_3.txt");
    std::cout << "Puzzle res pt 1: " << res.first << " pt 2: " << res.second << "\n";

    return 0;
}