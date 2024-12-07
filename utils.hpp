#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <type_traits>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <ranges>
#include <chrono>

std::string string_replace(std::string input, const std::string to_repl, const std::string repl){
    size_t pos_find = input.find(to_repl);
    while(pos_find != std::string::npos){
        input.replace(pos_find, to_repl.size(), repl);
        pos_find = input.find(to_repl, pos_find + repl.size());
    }
    return input;
}

std::vector<std::string> read_file_as_lines(const std::string filename, const int reserve_inner=1000){
    std::vector<std::string> lines;
    lines.reserve(reserve_inner);
    std::ifstream file(filename);
    std::string curr_line;
    while(std::getline(file, curr_line)){
        lines.push_back(curr_line);
    }
    return lines;
}

std::vector<std::vector<std::string>> read_file_as_blocks_lines(const std::string filename, const int reserve_inner=1000){
    std::vector<std::vector<std::string>> lines_vecs;
    std::vector<std::string> lines;
    lines.reserve(reserve_inner);
    std::ifstream file(filename);
    std::string curr_line;
    while(std::getline(file, curr_line)){
        if(curr_line.empty()){ // shove curr lines block into outermost vector, start new vector
            lines_vecs.push_back(lines);
            lines.clear();
            continue;
        }
        lines.push_back(curr_line);
    }
    //add last block
    lines_vecs.push_back(lines);
    return lines_vecs;
}

template <typename T>
T split_string_int(const std::string input, const char sep = ' ', const std::string repl = ""){
    T output;
    std::stringstream ss(input);
    for(auto& c : repl){
        auto tmp_str = ss.str();
        tmp_str = string_replace(tmp_str, std::string(1, c), std::string(1, sep));
        ss.str(tmp_str);
    }
    std::string str;
    while(std::getline(ss, str, sep)){
        if(str == "") continue; //with this this should (tm) work for an arbitrary number of spaces
        output.push_back(std::stoll(str));
    }
    return output;
}

std::vector<int>split_string_int(const std::string input, const char sep = ' ', const std::string repl = ""){
    return split_string_int<std::vector<int>>(input, sep, repl);
}

template<typename T, typename... Ts>
auto profile_function(T& function, double& time, Ts&... args){
    auto start = std::chrono::high_resolution_clock::now();
    auto ret = function(std::forward<Ts>(args)...);
    auto stop = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    return ret;
}

// printing utils
void print(){
    std::flush(std::cout);
    return;
}

//direct printing: only allowed for trivially printable types: integral types, floating point types, 
template <typename T, 
    typename std::enable_if<
        std::is_integral<T>::value || 
        std::is_floating_point<T>::value ||
        std::is_same<T, const char*>::value ||
        std::is_same<T, char>::value ||
        std::is_same<T, std::string>::value, bool>::type = true, 
    typename... Ts>
void print(T arg, Ts... arg2){
    std::cout << arg << "";
    print(arg2...);
}

void print(bool in){
    (in) ? std::cout << "true" : std::cout << "false";
}

//specialization for vector of trivially printable types
template <typename T, typename... Ts>
void print(std::vector<T> arg, Ts... arg2){
    std::cout << "vector type \n";
    for(auto const& elem: arg) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
    print(arg2...);
}

//specialization for set of trivially printable types
template <typename T, typename... Ts>
void print(std::set<T> arg, Ts... arg2){
    std::cout << "set type \n";
    for(auto const& elem: arg) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
    print(arg2...);
}
template <typename T, typename... Ts>
void print(std::unordered_set<T> arg, Ts... arg2){
    std::cout << "set type \n";
    for(auto const& elem: arg) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
    print(arg2...);
}

//specialization of map / unordered map of trivially printable types
template <typename T, typename U, typename... Ts>
void print(std::map<T, U> arg, Ts... arg2){
    std::cout << "map type, printing key, value in each line: \n";
    for (const auto& n : arg){
        print(n.first, n.second);
    }
    std::cout << "end of map \n" << std::endl;
    print(arg2...);
}

template <typename T, typename U, typename... Ts>
void print(std::unordered_map<T, U> arg, Ts... arg2){
    std::cout << "map type, printing key, value in each line: \n";
    for (const auto& n : arg){
        print(n.first, n.second);
    }
    std::cout << "end of map \n" << std::endl;
    print(arg2...);
}