#include "utils.hpp"
#include <functional>

// uint32_t binary_and(const std::string& s1, const std::string& s2){
//     return std::stoi(s1) & std::stoi(s2);
// }

// uint32_t binary_or(const std::string& s1, const std::string& s2){
//     return std::stoi(s1) | std::stoi(s2);
// }

// uint32_t binary_xor(const std::string& s1, const std::string& s2){
//     return std::stoi(s1) ^ std::stoi(s2);
// }

typedef std::map<std::string, std::vector<std::string>> opmap;

uint32_t get_value(const std::string& name, const opmap& map_ops, std::map<std::string, uint32_t>& map_vals){
    if(map_vals.contains(name)) return map_vals[name];
    std::function<uint32_t(const uint32_t&, const uint32_t&)> f;
    auto line_eval = map_ops.at(name);
    auto val1 = get_value(line_eval[0], map_ops, map_vals);
    auto val2 = get_value(line_eval[2], map_ops, map_vals);
    uint32_t ret;
    if(line_eval[1] == "AND") ret = std::bit_and<uint32_t>{}(val1, val2);
    else if(line_eval[1] == "OR") ret = std::bit_or<uint32_t>{}(val1, val2);
    else if(line_eval[1] == "XOR") ret = std::bit_xor<uint32_t>{}(val1, val2);
    map_vals.emplace(name, ret);
    return ret;
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};

    std::map<std::string, uint32_t> map_vals;
    //map vals: val1, op, val2, res
    opmap map_ops;
    std::vector<std::string> zvals;

    auto data = read_file_as_blocks_lines(filename);
    for(auto& line: data[0]){
        std::string name = line.substr(0, 3);
        uint32_t val = std::stoi(line.substr(5, 1));
        map_vals.emplace(name, val);
    }

    for(auto& line: data[1]){
        std::stringstream ss(line);
        for(auto& c : "->"){
            auto tmp_str = ss.str();
            tmp_str = string_replace(tmp_str, std::string(1, c), std::string(1, ' '));
            ss.str(tmp_str);
        }
        std::string str;
        std::vector<std::string> output;
        while(std::getline(ss, str, ' ')){
            if(str == "") continue; //with this this should (tm) work for an arbitrary number of spaces
            output.push_back(str);
        }
        map_ops.emplace(output[3], output);
        if(output[3][0] == 'z') zvals.push_back(output[3]);
    }

    std::sort(zvals.rbegin(), zvals.rend());
    for(auto& z: zvals){
        res.first = res.first << 1;
        res.first += get_value(z, map_ops, map_vals);
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_24.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_24.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}