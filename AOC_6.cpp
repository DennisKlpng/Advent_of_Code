#include "utils.hpp"
#include <execution>

//convention used: 
//- cartesian coordinate system (aka "right" = positive x and "up" = positive y)
//- obstacles in map are "1", free spaces are "0" 

typedef struct{
    int x = 0;
    int y = 0;
    int dx = 0;
    int dy = 1;
}guard_pos;

constexpr auto hash_pos(const int& x, const int&y){
    return 150*x + y;
}

// in pos x direction: 0, in neg y: 1, in neg x: 2, in pos y: 3
constexpr int hash_direction(const int& x, const int& y){
    if (x == 1) return 0;
    if (y == -1) return 1;
    if (x == -1) return 2;
    if (y == 1) return 3;
    return 0;
}

//returns true if guard is still in field, false if out of bounds
//hashing vector-index from position, value is if it has been visited in that direction
bool guard_move(guard_pos& pos, const std::vector<int>& arr, bool& loop_detected,
                std::vector<std::array<bool, 4>>& visited_pos, const int& x_max, const int& y_max){
    int x_n = pos.x + pos.dx;
    int y_n = pos.y + pos.dy;
    int pos_hash = hash_pos(x_n, y_n);
    if(x_n < 0 || x_n > x_max || y_n < 0 || y_n > y_max) return false;
    if(arr[pos_hash] == 1){
        //rotate with matrix ((0, 1), (-1, 0)) => x = y, y = - x
        int tmp = pos.dx;
        pos.dx = pos.dy;
        pos.dy = -tmp;
    }
    else{
        //move
        pos.x = x_n;
        pos.y = y_n;
        int dir_hash = hash_direction(pos.dx, pos.dy);
        if(visited_pos.at(pos_hash)[dir_hash] == true){
            loop_detected = true;
        }
        else{
            visited_pos.at(pos_hash)[dir_hash] = true;
        }
    }
    return true;
}

std::pair<int, int> solve_puzzle(std::string filename){
    std::pair<int, int> res{0, 0};
    std::vector<std::string> vec_input = read_file_as_lines(filename);

    const int x_max = vec_input[0].size() - 1;
    const int y_max = vec_input.size() -1;
    std::vector<int> arr;
    arr.resize(hash_pos(x_max, y_max));
    std::vector<std::array<bool, 4>> visited(hash_pos(x_max, y_max), {false, false, false, false});
    guard_pos start_pos;
    for(int y = 0; y <= y_max; y++){
        std::string line = vec_input.at(y_max - y); // get the bottommost line first, which equals to y = 0
        for(int x = 0; x <= x_max; x++){
            if (line[x] == '.') continue;
            else if (line[x] == '#') arr[hash_pos(x, y)] = 1;
            else if (line[x] == '^'){ 
                //guard faces up in both test and my data, could easily be generalized to every initial orientation
                start_pos.x = x;
                start_pos.y = y;
                visited[hash_pos(x, y)][hash_direction(0, 1)] = true;
            }
        }
    }
    std::set<int> pos_visited_pt1{hash_pos(start_pos.x, start_pos.y)};
    //pos_visited_pt1.reserve(hash_pos(x_max, y_max));
    {
        guard_pos pos_update = start_pos;
        std::vector<std::array<bool, 4>> visited_local = visited;
        bool loop = false;
        while(guard_move(pos_update, arr, loop, visited_local, x_max, y_max)){
            pos_visited_pt1.insert(hash_pos(pos_update.x, pos_update.y));
        };
        res.first = pos_visited_pt1.size(); //+1 because of start pos
    }

    //part 2 semi brute-force: try to place an obstacle on every position (besides the start) that the guard visited in 1
    std::mutex num_mutex;
    std::for_each(std::execution::par, pos_visited_pt1.begin(), pos_visited_pt1.end(),[&](auto pos_pt_1){
        //check if there's an obstacle in the way
        guard_pos pos_loop = start_pos;
        bool loop = false;
        std::vector<int> arr_new = arr;
        arr_new[pos_pt_1] = 1;

        std::vector<std::array<bool, 4>> visited_loop = visited;
        while(guard_move(pos_loop, arr_new, loop, visited_loop, x_max, y_max)){
            if(loop){
                num_mutex.lock();
                res.second += 1;
                num_mutex.unlock();
                break;
            }
        };
    });

    return res;
}

int main(){
    std::pair<int, int> res = solve_puzzle("inputs/Test_6.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_6.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}