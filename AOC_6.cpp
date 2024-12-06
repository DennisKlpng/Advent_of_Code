#include "utils.hpp"
#include "unordered_set"

//convention used: 
//- cartesian coordinate system (aka "right" = positive x and "up" = positive y)
//- obstacles in map are "1", free spaces are "0" 

typedef struct{
    int x = 0;
    int y = 0;
    int dx = 0;
    int dy = 1;
}guard_pos;

struct hash_coords {
    inline std::size_t operator()(const std::pair<int, int>& coords_to_hash) const {
        return 150*coords_to_hash.first + coords_to_hash.second;
    }
};

//returns true if guard is still in field, false if out of bounds
bool guard_move(guard_pos& pos, const std::vector<std::vector<int>>& arr, 
                std::unordered_set<std::pair<int, int>, hash_coords>& visited_pos, const int& x_max, const int& y_max){
    int x_n = pos.x + pos.dx;
    int y_n = pos.y + pos.dy;
    if(x_n < 0 || x_n > x_max || y_n < 0 || y_n > y_max) return false;
    if(arr[x_n][y_n] == 1){
        //rotate with matrix ((0, 1), (-1, 0)) => x = y, y = - x
        int tmp = pos.dx;
        pos.dx = pos.dy;
        pos.dy = -tmp;
    }
    else{
        //move
        pos.x = x_n;
        pos.y = y_n;
        visited_pos.emplace(x_n, y_n);
    }
    return true;
}

std::pair<int, int> solve_puzzle(std::string filename){
    std::pair<int, int> res{0, 0};
    std::vector<std::string> vec_input = read_file_as_lines(filename);

    const int x_max = vec_input[0].size() - 1;
    const int y_max = vec_input.size() -1;
    std::vector<std::vector<int>> arr(x_max + 1, std::vector<int>(y_max + 1, 0));
    std::unordered_set<std::pair<int, int>, hash_coords> visited;
    std::pair<int, int> start_pos;
    visited.reserve(x_max*y_max); //avoid rehashing
    guard_pos pos;
    for(int y = 0; y <= y_max; y++){
        std::string line = vec_input.at(y_max - y); // get the bottommost line first, which equals to y = 0
        for(int x = 0; x <= x_max; x++){
            if (line[x] == '.') continue;
            else if (line[x] == '#') arr[x][y] = 1;
            else if (line[x] == '^'){ 
                //guard faces up in both test and my data, could easily be generalized to every initial orientation
                pos.x = x;
                pos.y = y;
                start_pos.first = x;
                start_pos.second = y;
                visited.emplace(x, y);
            }
        }
    }

    guard_pos pos_update = pos;
    while(guard_move(pos_update, arr, visited, x_max, y_max));
    res.first = visited.size();

    //part 2 semi brute-force: try to place an obstacle on every position (besides the start) that the guard visited in 1
    visited.erase(start_pos);
    // for (auto& pos : visited){
    //     std::vector<std::vector<int>> arr_new = arr;
    //     arr_new[pos.first][pos.second] = 1;
    // }

    return res;
}

int main(){
    std::pair<int, int> res = solve_puzzle("Test_6.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");

    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "Data_6.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}