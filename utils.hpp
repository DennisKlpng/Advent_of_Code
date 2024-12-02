#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <type_traits>
#include <map>
#include <unordered_map>

std::vector<std::string> read_file_as_lines(const std::string filename){
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string curr_line;
    while(std::getline(file, curr_line)){
        lines.push_back(curr_line);
    }
    return lines;
}

std::vector<int> split_string_int(const std::string input){
    std::vector<int> output;
    std::stringstream ss(input);
    std::string str;
    while(std::getline(ss, str, ' ')){
        if(str == "") continue; //with this this should (tm) work for an arbitrary number of spaces
        output.push_back(std::stoi(str));
    }
    return output;
}

void print(){
    std::cout << std::endl;
    return;
}

//direct printing: only allowed for trivially printable types: integral types, floating point types, 
template <typename T, 
    typename std::enable_if<
        std::is_integral<T>::value || 
        std::is_floating_point<T>::value ||
        std::is_same<T, std::string>::value, bool>::type = true, 
    typename... Ts>
void print(T arg, Ts... arg2){
    std::cout << arg << " ";
    print(arg2...);
}

//specialization for vector of trivially printable types
template <typename T, typename... Ts>
void print(std::vector<T> arg, Ts... arg2){
    std::cout << "vector type: \n";
    for(auto& i : arg){
        print(i);
    }
    std::cout << "end of vector" << std::endl;
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