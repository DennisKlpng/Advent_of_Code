#include "utils.hpp"

static int64_t max_x = 0;
static int64_t max_y = 0;

//x: right, y: down
typedef std::pair<int64_t, int64_t> pt;
constexpr pt operator+(const pt& pt1, const pt& pt2){
    return pt(pt1.first + pt2.first, pt1.second + pt2.second);
}

typedef enum mp{
    FREE = 0,
    WALL = 1,
    BOX = 2,
    ROB = 3
} mp;

typedef enum move{
    NONE = 0,
    UP = 1,
    RIGHT = 2,
    DOWN = 3,
    LEFT = 4
}move;

pt get_move_vec(const move mv){
    if(mv == UP) return pt(0, -1);
    if(mv == RIGHT) return pt(1, 0);
    if(mv == DOWN) return pt(0, 1);
    if(mv == LEFT) return pt(-1, 0);
    return pt(0, 0);
}

inline char get_elem(const mp obj){
    if(obj == mp::FREE) return '_';
    if(obj == mp::BOX) return 'O';
    if(obj == mp::WALL) return '#';
    return '@';
}

void visualize_map(std::map<pt, mp>& curr_map){
    std::string empt_line(max_x + 1, ' ');
    std::vector<std::string> vec_lines(max_y + 1, empt_line);

    for(auto& [k, v]: curr_map){
        vec_lines[k.second][k.first] = get_elem(v);
    }
    for(auto& line: vec_lines){
        print(line);
    }
}

bool try_move_box(const pt& pos, const move mv, std::map<pt, mp>& curr_map){
    auto tgt_pos = pos + get_move_vec(mv);
    if(curr_map[tgt_pos] == mp::WALL) return false;
    if(curr_map[tgt_pos] == mp::BOX){
        if(!try_move_box(tgt_pos, mv, curr_map)) return false;
    }
    curr_map[tgt_pos] = mp::BOX;
    curr_map[pos] = mp::FREE;
    return true;
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};

    auto lines = read_file_as_blocks_lines(filename);
    max_x = lines[0].at(0).size() - 1;
    max_y = lines[0].size() - 1;
    std::map<pt, mp> warehouse_map;
    pt robot_pos;
    int64_t y = 0;
    for(auto& line_map: lines[0]){
        int64_t x = 0;
        for(auto&c : line_map){
            if(c == '.') warehouse_map.try_emplace(pt(x, y), mp::FREE);
            else if(c == 'O') warehouse_map.try_emplace(pt(x, y), mp::BOX);
            else if(c == '#') warehouse_map.try_emplace(pt(x, y), mp::WALL);
            else if(c == '@') {
                warehouse_map.try_emplace(pt(x, y), mp::ROB);
                robot_pos = pt(x, y);
            }
            x++;
        }
        y++;
    }
    std::vector<move> moves;
    moves.reserve(std::max_element(lines[1].begin(), lines[1].end(), [](const auto& a, const auto& b){
        return a.size() < b.size();
    })->size());
    for(auto& line: lines[1]){
        for(auto& c: line){
            move curr;
            if(c=='^') curr = move::UP;
            if(c=='>') curr = move::RIGHT;
            if(c=='v') curr = move::DOWN;
            if(c=='<') curr = move::LEFT;
            moves.push_back(curr);
        }
    }

    //pt1:
    for(const move& mv : moves){
        // visualize_map(warehouse_map);
        pt new_pos = robot_pos + get_move_vec(mv);
        if(warehouse_map[new_pos] == mp::WALL) continue;
        if(warehouse_map[new_pos] == mp::BOX){
            if(!try_move_box(new_pos, mv, warehouse_map)) continue;
        }
        warehouse_map[robot_pos] = mp::FREE;
        warehouse_map[new_pos] = mp::ROB;
        robot_pos = new_pos;
    }
    // visualize_map(warehouse_map);
    for(const auto& [k, v] : warehouse_map){
        if(v == mp::BOX){
            res.first += k.first + 100*k.second;
        }
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_15.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_15.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}