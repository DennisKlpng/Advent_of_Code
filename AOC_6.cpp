#include "utils.hpp"

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
        return 150*coords_to_hash.first + coords_to_hash.second; //enough for all inputs we have to avoid collisions
    }
};

// in pos x direction: 1, in neg y: 2, in neg x: 3, in pos y: 4
int calc_diretion_hash(const int& x, const int& y){
    if (x == 1) return 1;
    if (y == -1) return 2;
    if (x == -1) return 3;
    if (y == 1) return 4;
    return 0;
}

//returns true if guard is still in field, false if out of bounds
bool guard_move(guard_pos& pos, const std::vector<std::vector<int>>& arr, bool& loop_detected,
                std::unordered_map<std::pair<int, int>, std::set<int>, hash_coords>& visited_pos, 
                const int& x_max, const int& y_max){
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
        int dir_hash = calc_diretion_hash(pos.dx, pos.dy);
        if(auto search = visited_pos.find(std::make_pair(x_n, y_n)); search != visited_pos.end()){
            // print("pos: ", x_n, " , ", y_n, "\n");
            // print(search->second);
            //pos was already visited, 
            //loop detection: loop if we visit the same position with the same orientation
            //check if hash for position already exists. If yes emplace returns false (as second of return pair)
            loop_detected = !search->second.emplace(dir_hash).second;
        }
        else{
            visited_pos.emplace(std::make_pair(x_n, y_n), std::set<int>{dir_hash});
        }
    }
    return true;
}

std::pair<int, int> solve_puzzle(std::string filename){
    std::pair<int, int> res{0, 0};
    std::vector<std::string> vec_input = read_file_as_lines(filename);

    const int x_max = vec_input[0].size() - 1;
    const int y_max = vec_input.size() -1;
    std::vector<std::vector<int>> arr(x_max + 1, std::vector<int>(y_max + 1, 0));
    std::unordered_map<std::pair<int, int>, std::set<int>, hash_coords> visited;
    visited.reserve(150*150); //reserving to avoid rehashes
    std::pair<int, int> start_pos;
    //visited.reserve(x_max*y_max); //avoid rehashing
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
                // print("startpos: ", x, " , ", y, "\n");
                visited.emplace(std::make_pair(x, y), std::set<int>{calc_diretion_hash(pos.dx, pos.dy)});
            }
        }
    }

    {
        guard_pos pos_update = pos;
        bool loop = false;
        while(guard_move(pos_update, arr, loop, visited, x_max, y_max));
        res.first = visited.size();
    }

    //part 2 semi brute-force: try to place an obstacle on every position (besides the start) that the guard visited in 1
    visited.erase(start_pos);
    for (auto& [obst_candidate, v] : visited){
        guard_pos pos_loop = pos;
        bool loop = false;
        std::vector<std::vector<int>> arr_new = arr;
        arr_new[obst_candidate.first][obst_candidate.second] = 1;

        std::unordered_map<std::pair<int, int>, std::set<int>, hash_coords> visited_loop;
        while(guard_move(pos_loop, arr_new, loop, visited_loop, x_max, y_max)){
            if(loop){
                res.second += 1;
                break;
            }
        };
    }

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