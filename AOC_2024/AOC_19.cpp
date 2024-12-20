#include "utils.hpp"

uint64_t make_design(const std::vector<std::string>& patterns, std::map<std::string, uint64_t>& hash,
        const std::string& design, const bool pt2 = false){
    if(design.empty()) return 1;
    if(hash.contains(design)) return hash[design];
    uint64_t size = 0;
    for (auto &&pat : patterns){
        if(design.starts_with(pat)){
            std::string sub = design.substr(pat.size());
            if(pt2){
                size += make_design(patterns, hash, sub, pt2);
            }
            else{
                size = std::max(size, make_design(patterns, hash, sub, pt2));
            }
        }
    }
    hash.emplace(design, size);
    
    return size;
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};

    auto blocks = read_file_as_blocks_lines(filename);

    std::vector<std::string> patterns;
    std::stringstream ss(blocks[0][0]);

    auto tmp_str = ss.str();
    tmp_str = string_replace(tmp_str, std::string(1, ' '), "");
    ss.str(tmp_str);
    std::string str;
    while(std::getline(ss, str, ',')){
        if(str == "") continue; //with this this should (tm) work for an arbitrary number of spaces
        patterns.push_back(str);
    }

    auto designs = blocks[1];
    std::map<std::string, uint64_t> hashes;
    for(auto& design: designs){
        res.first += make_design(patterns, hashes, design);
    }
    hashes.clear();
    for(auto& design: designs){
        res.second += make_design(patterns, hashes, design, true);
    }

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_19.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_19.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}