#include "utils.hpp"
#include <fstream>
#include <filesystem>

static int64_t max_x = 0;
static int64_t max_y = 0;

const static bool save_steps = true;

typedef enum mp{FREE = 0, WALL = 1, BOX = 2, ROB = 3, BOX_LEFT = 4, BOX_RIGHT = 5} mp;
typedef enum move{UP = 1, RIGHT = 2, DOWN = 3, LEFT = 4}move;

//x: right, y: down
typedef std::pair<int64_t, int64_t> pt;
typedef std::pair<pt, mp> pt_with_type;

constexpr pt operator+(const pt& pt1, const pt& pt2){
    return pt(pt1.first + pt2.first, pt1.second + pt2.second);
}

constexpr pt get_move_vec(const move mv){
    if(mv == UP) return pt(0, -1);
    if(mv == RIGHT) return pt(1, 0);
    if(mv == DOWN) return pt(0, 1);
    if(mv == LEFT) return pt(-1, 0);
    return pt(0, 0);
}

constexpr char get_elem(const mp obj){
    if(obj == mp::FREE) return '.';
    if(obj == mp::BOX) return 'O';
    if(obj == mp::WALL) return '#';
    if(obj == mp::BOX_LEFT) return '[';
    if(obj == mp::BOX_RIGHT) return ']';
    return '@';
}

void visualize_map(const std::map<pt, mp>& curr_map, const bool pt2=false){
    int factor = 1;
    if(pt2) factor = 2;
    std::string empt_line(factor*(max_x + 1), ' ');
    std::vector<std::string> vec_lines(max_y + 1, empt_line);

    for(const auto& [k, v]: curr_map){
        vec_lines[k.second][k.first] = get_elem(v);
    }
    for(const auto& line: vec_lines){
        print(line);
    }
    print("");
}

void save_map(const std::map<pt, mp>& curr_map, const int& step){
    std::ofstream file;
    std::string filename = "./steps/step_" + std::to_string(step) + ".txt";
    file.open(filename);
    std::string empt_line(2*(max_x + 1), ' ');
    std::vector<std::string> vec_lines(max_y + 1, empt_line);
    for(const auto& [k, v]: curr_map){
        vec_lines[k.second][k.first] = get_elem(v);
    }
    for(const auto& line: vec_lines){
        file << line << "\n";
    }
    file.close();
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

bool try_move_large_box(const pt& pos, const move mv, std::map<pt, mp>& curr_map, std::set<pt_with_type>& move_pos, bool ignore_nb=false){
    // auto tmp_map = curr_map;
    auto curr_type = curr_map[pos];
    auto tgt_pos = pos + get_move_vec(mv);
    if(curr_map[tgt_pos] == mp::WALL) return false;
    if(curr_map[tgt_pos] == mp::BOX_LEFT || curr_map[tgt_pos] == mp::BOX_RIGHT){
        if(!try_move_large_box(tgt_pos, mv, curr_map, move_pos)) return false;
    }
    if((mv == move::UP || mv == move::DOWN) && !ignore_nb){ //also consider the other part of the box to push if we didn't already
        if(curr_type == mp::BOX_LEFT){ //also try to push the BOX_RIGHT just right of the box part we want to push
            if(!try_move_large_box(pos + pt(1, 0), mv, curr_map, move_pos, true)) return false;
            move_pos.emplace(pos + pt(1, 0), mp::BOX_RIGHT);
        }
        else if(curr_type == mp::BOX_RIGHT){ //also try to push the BOX_LEFT just left of the box part we want to push
            if(!try_move_large_box(pos + pt(-1, 0), mv, curr_map, move_pos, true)) return false;
            move_pos.emplace(pos + pt(-1, 0), mp::BOX_LEFT);
        }
    }

    move_pos.emplace(pos, curr_type);
    return true;
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};

    auto lines = read_file_as_blocks_lines(filename);
    max_x = lines[0].at(0).size() - 1;
    max_y = lines[0].size() - 1;
    std::map<pt, mp> warehouse_map;
    std::map<pt, mp> warehouse_map_2;
    pt robot_pos;
    pt robot_pos_2;
    int64_t y = 0;
    for(const auto& line_map: lines[0]){
        int64_t x = 0;
        for(const auto&c : line_map){
            if(c == '.') {
                warehouse_map.try_emplace(pt(x, y), mp::FREE);
                warehouse_map_2.try_emplace(pt(2*x, y), mp::FREE);
                warehouse_map_2.try_emplace(pt(2*x + 1, y), mp::FREE);
            }
            else if(c == 'O'){
                warehouse_map.try_emplace(pt(x, y), mp::BOX);
                warehouse_map_2.try_emplace(pt(2*x, y), mp::BOX_LEFT);
                warehouse_map_2.try_emplace(pt(2*x + 1, y), mp::BOX_RIGHT);
            }
            else if(c == '#'){
                warehouse_map.try_emplace(pt(x, y), mp::WALL);
                warehouse_map_2.try_emplace(pt(2*x, y), mp::WALL);
                warehouse_map_2.try_emplace(pt(2*x + 1, y), mp::WALL);
            }
            else if(c == '@') {
                warehouse_map.try_emplace(pt(x, y), mp::ROB);
                warehouse_map_2.try_emplace(pt(2*x, y), mp::ROB);
                warehouse_map_2.try_emplace(pt(2*x+1, y), mp::FREE);
                robot_pos = pt(x, y);
                robot_pos_2 = pt(2*x, y);
            }
            x++;
        }
        y++;
    }
    std::vector<move> moves;
    moves.reserve(std::ranges::max_element(lines[1], [](const auto& a, const auto& b){
        return a.size() < b.size();
    })->size());
    for(const auto& line: lines[1]){
        for(const auto& c: line){
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

    //pt2
    int step = 0;
    if(save_steps){
        std::filesystem::remove_all("./steps");
        std::filesystem::create_directory("./steps");
    }
    for(const move& mv : moves){
        pt new_pos = robot_pos_2 + get_move_vec(mv);
        if(warehouse_map_2[new_pos] == mp::WALL) continue;
        if(warehouse_map_2[new_pos] == mp::BOX_LEFT || warehouse_map_2[new_pos] == mp::BOX_RIGHT){
            std::set<pt_with_type> move_pos;
            if(!try_move_large_box(new_pos, mv, warehouse_map_2, move_pos)) continue;
            for(auto& pos: move_pos){ //firt pass: delete old positions
                warehouse_map_2[pos.first] = mp::FREE;
            }
            for(auto& pos: move_pos){ //second pass: add new positions
                auto tgt_pos = pos.first + get_move_vec(mv);
                warehouse_map_2[tgt_pos] = pos.second;
            }
        }
        warehouse_map_2[robot_pos_2] = mp::FREE;
        warehouse_map_2[new_pos] = mp::ROB;

        if(save_steps){
            step++;
            save_map(warehouse_map_2, step);
        }

        robot_pos_2 = new_pos;
    }

    for(const auto& [k, v] : warehouse_map_2){
        if(v == mp::BOX_LEFT){
            res.second += k.first + 100*k.second;
        }
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_15.txt");
    // std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_15_easy2.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    // res = profile_function(solve_puzzle, time_spent, "inputs/Data_15.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}