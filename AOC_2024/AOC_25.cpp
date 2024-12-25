#include "utils.hpp"
#include <numeric>

typedef std::vector<int16_t> pattern;
std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};

    auto data = read_file_as_blocks_lines(filename);
    int16_t height = data[0].size();
    int16_t width = data[0][0].size();
    
    std::vector<pattern> locks;
    std::vector<pattern> keys; 
    
    for(auto& block: data){
        if(block[0][0] == '#'){ //lock
            pattern lock(width, 0);
            for(int16_t i = 0; i < height; i++){
                for(int16_t j = 0; j < width; j++){
                    if(block[i][j] == '#'){
                        lock[j] = i+1;
                    }
                }
            }
            locks.push_back(lock);
        }
        else{
            pattern key(width, 0);
            for(int16_t i = 0; i < height; i++){
                for(int16_t j = 0; j < width; j++){
                    if(block[i][j] == '.'){
                        key[j] = height - (i + 1);
                    }
                }
            }
            keys.push_back(key);
        }
    }

    for(auto&k : keys){
        for(auto& l: locks){
            auto sum = std::views::zip_transform(std::plus<int16_t>(), k, l);
            if(std::all_of(sum.cbegin(), sum.cend(), [&](auto n){return n <= height;})) res.first++;
        }
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_25.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_25.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}