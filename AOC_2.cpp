#include "utils.hpp"
#include <algorithm>
#include <numeric>
#include <unordered_map>

int check_order(int a, int b){
    if((a != b) && (abs(b-a) <= 3)) return (b-a)/abs(b-a); //+1 if increasing, -1 if decreasing
    return 0;
}

bool check_if_safe(std::vector<int> nums){
    std::vector<int> pair_orders;
    pair_orders.resize(nums.size() - 1);
    for(int n = 0; n < nums.size() - 1; n++){
        pair_orders[n] = check_order(nums[n], nums[n+1]);
    }
    int fst= pair_orders[0];
    //check if all pairs have the same ordering as the first one (and that this isn't 0, aka equal)
    if((fst != 0) && std::all_of(pair_orders.cbegin(), pair_orders.cend(), [&fst](int i) { return (fst == i); })
    ){
        return true;
    }
    return false;
}

std::pair<int, int> solve_puzzle(std::string filename){
    std::pair<int, int> res{0, 0};

    std::vector<std::string> lines = read_file_as_lines(filename);
    for(auto& n : lines){
        //split each line into vector of ints, stuff them into separate vectors
        std::vector<int> ints_line = split_string_int(n);
        //pt 1:
        if(check_if_safe(ints_line)) res.first++;
        //pt 2 brute force: try with each num for each line removed, doesn't make that much of a difference with the small inputs
        for(int i = 0; i < ints_line.size(); i++){
            std::vector<int> new_vec = ints_line;
            new_vec.erase(new_vec.begin() + i);
            if(check_if_safe(new_vec)){
                res.second++;
                break;
            }
        }
    }

    return res;
}

int main(){
    std::pair<int, int> res = solve_puzzle("Test_2.txt");
    std::cout << "TEST res pt 1: " << res.first << " pt 2: " << res.second << "\n";
    res = solve_puzzle("Data_2.txt");
    std::cout << "Puzzle res pt 1: " << res.first << " pt 2: " << res.second << "\n";

    return 0;
}