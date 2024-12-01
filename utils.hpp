#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

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
    return;
}

//only use for basic types, doesn't check currently!
template <typename T, typename... Ts>
void print(T arg, Ts... arg2){
    std::cout << arg << " " << std::endl;
    print(arg2...);
}