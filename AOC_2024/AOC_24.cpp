#include "utils.hpp"
#include <functional>
#include <cmath>

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

int64_t solve_adder(const std::vector<std::string>& zvals, const opmap& map_ops, 
                     std::map<std::string, uint32_t> map_vals){
    int64_t res = 0;
    for(auto& z: zvals){
        res = res << 1;
        res += get_value(z, map_ops, map_vals);
    }
    return res;
}

std::pair<uint64_t, std::string> solve_puzzle(std::string filename, bool pt2 = false){
    std::pair<uint64_t, std::string> res{0, ""};

    std::map<std::string, uint32_t> map_vals;
    //map vals: val1, op, val2, res
    opmap map_ops;
    std::vector<std::string> zvals;

    int64_t x = 0;
    int64_t y = 0;

    auto data = read_file_as_blocks_lines(filename);
    for(auto& line: data[0]){
        std::string name = line.substr(0, 3);
        int64_t index = std::stoull(line.substr(1,2));
        int64_t val = std::stoull(line.substr(5, 1));
        if(line[0] == 'x') x += (val << index);
        if(line[0] == 'y') y += (val << index);
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
    res.first = solve_adder(zvals, map_ops, map_vals);
    if(!pt2) return res;

    /* 
        part 2: the whole thing is a ripple-carry adder
        On the least significant bit, we half a half adder: x00 XOR y00 = z00 and x00 AND y00 = carry_out_0
        On the n-bits behind that we have:
            xn XOR yn = tmp_n1 XOR carry_in_n = zn
            xn AND yn = tmp_n3
            tmp_n1 AND carry_in_n = tmp_n2
            tmp_n2 OR tmp_n3 = carry_out_n
        On the most significant bit zn = carry_in_n
        => any zn must come from an XOR for every n
        there must be an xn AND yn and an xn XOR yn for every n
        In addition each OR must have two ANDs as predecessors
        Each XOR has as predecessors: either xm and ym or OR + XOR (for n>1, the half adder at n=0 has AND)
     */
    std::vector<std::vector<std::string>> errors;
    std::vector<std::vector<std::string>> errors_XOR;
    std::vector<std::vector<std::string>> errors_zres;
    for(auto& [k,v] : map_ops){
        if(k[0] == 'z' && v[1] != "XOR"){
            //cases where the result is zn and therefore should be the XOR of cn and tmpn1, but op is no XOR
            if(k == zvals[0]) continue; //most significant bit is an OR comp
            errors.push_back(v);
            errors_zres.push_back(v);
        }
        if((v[0][0] != 'x' && v[0][0] != 'y') && v[1] == "XOR" && k[0] != 'z'){
            //cases where we XOR cn and tmpn1, but the result is not zn
            errors.push_back(v);
            errors_XOR.push_back(v);
        }
    }
    //this yields 6 falsely wired gates (3 which violate criterion 1 and 3 which violate 2). 
    //next associate these gates into pairs and swap them
    for(auto& err_xor : errors_XOR){
        int m = -1;
        for(int i: {0, 2}){
            auto val = map_ops[err_xor[i]];
            if(val[0][0] == 'x' || val[0][0] == 'y'){
                //the first (i=0) / second (i=2) input that causes the xor error is built from some xm and xm
                //=> this imput is tmpn_1 with n = m (the other is cn)
                //extract m, afterwards match it to find the correct zm it creates
                m = std::stoi(map_ops[err_xor[i]][0].substr(1, 2));
            }
        }
        assert(m > 0);
        for(auto& err_zres: errors_zres){
            if(err_zres[3] == "z" + std::to_string(m)){
                //swap the xor-error val and the according zres-error val
                for(int j: {0,1,2}){
                    map_ops[err_xor[3]][j] = err_zres[j];
                    map_ops[err_zres[3]][j] = err_xor[j];
                }
            }
        }
    }
    //after fixing the formerly wrong gates it's now possible to also check successors / predecessors
    for(auto& [k,v] : map_ops){
        if(v[1] == "OR"){
            //check that each OR only has ANDs as predecessors
            if(map_ops[v[0]][1] != "AND"){
               errors.push_back(map_ops[v[0]]);
            }
            if(map_ops[v[2]][1] != "AND"){
                errors.push_back(map_ops[v[2]]);
            }
        }
        if(v[1] == "XOR"){
            //check for each full adder that am XOR never has AND as predecessor
            //(we have to ignore n=0, aka when processing x00 and y00, since n=0 is a half adder 
            //where the carry is x00 AND y00)
            if(v[0][0] != 'x' && v[0][0] != 'y'){
                auto pred = map_ops[v[0]];
                if(pred[1] == "AND" && pred[0].substr(1, 2) != "00"){
                    errors.push_back(pred);
                }
                pred = map_ops[v[2]];
                if(pred[1] == "AND" && pred[0].substr(1, 2) != "00"){
                    errors.push_back(map_ops[v[2]]);
                }
            }
        }
    }
    std::sort(errors.begin(), errors.end(),[](auto& a, auto& b){return a[3] < b[3];});
    for(auto& s: errors){
        res.second += s[3] + ",";
    }

    return res;
}

int main(){
    std::pair<uint64_t, std::string> res = solve_puzzle("inputs/Test_24.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    bool pt2 = true;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_24.txt", pt2);
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}