#include "utils.hpp"
#include <numeric>

static int64_t width = 0;
static int64_t height = 0;

//top_left: 0, top_right: 1, bottom_left: 2, bottom_right: 3, 4: somewhere in the middle
inline int64_t get_quadrant(std::pair<int64_t, int64_t> pos){
    int64_t res = 0;
    if(pos.first == width/2 || pos.second == height/2) return 4;
    if(pos.second > height/2) res += 2;
    if(pos.first > width/2) res += 1;
    return res;
}

void visualize_state(std::vector<std::string> curr_state){
    for(auto& line: curr_state){
        print(line, "\n");
    }
}

struct pt{
    pt(int64_t x_0, int64_t y_0, int64_t v_x, int64_t v_y): x_0(x_0), y_0(y_0), v_x(v_x), v_y(v_y){};

    int64_t x_0 = 0;
    int64_t y_0 = 0;
    int64_t v_x = 0;
    int64_t v_y = 0;

    std::pair<int64_t, int64_t> get_pos(const int64_t moves/*, const int64_t max_x, const int64_t max_y*/){
        std::pair<int64_t, int64_t> res;
        //in c++ we have to calc a%b = (a%b + b)%b if a can be negative
        res.first = ((x_0 + moves * v_x)%width + width)%width;
        res.second = ((y_0 + moves * v_y)%height + height)%height;
        return res;
    }
};

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename, bool is_test = true){
    std::pair<uint64_t, uint64_t> res{0, 0};
    
    if(is_test){
        width = 11;
        height = 7;
    }
    else{
        width = 101;
        height = 103;
    }
    auto lines = read_file_as_lines(filename);
    std::vector<pt> vec_robots;
    vec_robots.reserve(lines.size());
    for(auto& l: lines){
        auto coords = get_ints_from_string(l);
        pt robot(coords[0], coords[1], coords[2], coords[3]);
        vec_robots.push_back(robot);
    }

    //part 2, what the
    int64_t min_safety = std::numeric_limits<int64_t>::max();
    std::string emptry_line(width, ' ');
    std::vector<std::string> visualizer(height, emptry_line);
    for(int64_t i = 0; i <= width*height; i++){
        auto vis_local = visualizer;
        std::vector<int64_t> quadrant_robots{0, 0, 0, 0, 0};
        std::for_each(vec_robots.begin(), vec_robots.end(), [&](pt& robot){
            auto pos = robot.get_pos(i);
            quadrant_robots[get_quadrant(pos)] += 1;
            vis_local[pos.second][pos.first] = 'X';
        });
        int64_t current_safety = std::accumulate(quadrant_robots.begin(), quadrant_robots.end() - 1, 1, std::multiplies<int64_t>());
        if(i == 100) res.first = current_safety;
        if(current_safety < min_safety && i > 100) {
            min_safety = current_safety;
            res.second = i;
            //optional visualization
            // visualize_state(vis_local);
            // print(i, " ##############################################################################################################", "\n");
        }
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_14.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");
    double time_spent;
    bool test = false;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_14.txt", test);
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}