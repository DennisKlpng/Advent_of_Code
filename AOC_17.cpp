#include "utils.hpp"
#include <cmath>

uint64_t get_combo(uint64_t operand, uint64_t& A, uint64_t& B, uint64_t& C){
    if(operand <= 3){
        return operand;
    }
    if(operand == 4) return A;
    if(operand == 5) return B;
    if(operand == 6) return C;
    if(operand == 7) print("ERROR invalid combo operand 7"); 
    return -1;
}

uint64_t bitwise_xor(uint64_t x, uint64_t y){
    return x ^ y;
}

std::string parse_instr(uint64_t opcode, uint64_t operand, uint64_t& A, uint64_t& B, uint64_t& C, uint64_t& n){
    std::string res("");
    switch(opcode){
        case(0):  //adv
            A = A / pow(2, get_combo(operand, A, B, C));
            break;
        case(1):  //bxl
            B = bitwise_xor(B, operand);
            break;
        case(2): //bst
            B = get_combo(operand, A, B, C)%8;
            break;
        case(3): //jnz
            if(A == 0) break;
            n = operand;
            return res;
        case(4): //bxc
            B = bitwise_xor(B, C);
            break;
        case(5): //out
            res = std::to_string(get_combo(operand, A, B, C) % 8);
            break;
        case(6): //bdv
            B = A / pow(2, get_combo(operand, A, B, C));
            break;
        case(7): //cdv
            C = A / pow(2, get_combo(operand, A, B, C));
            break;
    }
    n += 2;
    return res;
}

std::pair<std::string, uint64_t> solve_puzzle(std::string filename){
    std::pair<std::string, uint64_t> res{"", 0};

    uint64_t A, B, C;

    auto data = read_file_as_blocks_lines(filename);
    A = get_ints_from_string(data[0][0])[0];
    B = get_ints_from_string(data[0][1])[0];
    C = get_ints_from_string(data[0][2])[0];

    auto instructions = get_ints_from_string(data[1][0]);

    for(uint64_t n = 0; n < instructions.size();){
        std::string tmp = parse_instr(instructions[0+n], instructions[1+n], A, B, C, n);
        if(tmp != "") {
            if(res.first != "") res.first += ",";
            res.first += tmp;
        }
        if(n >= instructions.size()) break;
    }

    return res;
}

int main(){
    // {
    //     //Test cases
    //     assert(bitwise_xor(29, 7) == 26);
    //     uint64_t A, B, C, n;
    //     C = 9;
    //     parse_instr(2, 6, A, B, C, n);
    //     assert(B == 1);
    //     A = 10;
    //     assert("0" == parse_instr(5, 0, A, B, C, n));
    //     assert("1" == parse_instr(5, 1, A, B, C, n));
    //     assert("2" == parse_instr(5, 4, A, B, C, n));
    //     B = 29;
    //     parse_instr(1, 7, A, B, C, n);
    //     assert(B == 26);
    //     B = 2024;
    //     C = 43690;
    //     parse_instr(4, 0, A, B, C, n);
    //     assert(B == 44354);
    // }

    std::pair<std::string, uint64_t> res = solve_puzzle("inputs/Test_17.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_17.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}