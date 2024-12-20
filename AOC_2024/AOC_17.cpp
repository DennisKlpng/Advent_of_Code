#include "utils.hpp"
#include <cmath>
#include <deque>

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

std::string solve_code(std::vector<int64_t> instr, uint64_t A, uint64_t B, uint64_t C, const std::string sep=","){
    std::string res = "";
    for(uint64_t n = 0; n <= instr.size();){
        std::string tmp = parse_instr(instr[0+n], instr[1+n], A, B, C, n);
        if(tmp != "") {
            if(res != "") res += sep;
            res += tmp;
        }
        if(n >= instr.size()) break;
    }
    return res;
}

std::pair<std::string, int64_t> solve_puzzle(std::string filename){
    std::pair<std::string, int64_t> res{"", std::numeric_limits<int64_t>::max()};

    uint64_t A, B, C;

    auto data = read_file_as_blocks_lines(filename);
    A = get_ints_from_string(data[0][0])[0];
    B = get_ints_from_string(data[0][1])[0];
    C = get_ints_from_string(data[0][2])[0];

    auto instructions = get_ints_from_string(data[1][0]);
    res.first = solve_code(instructions, A, B, C);

    //part 2:
    /*
    - A is only modified by adv-operation => opcode 0
    - Last instruction is 3,0
    - => code completely loop until A becomes 0
    - print is always second last and prints get_combo(operand) (A for example, B for data)
    - we have one pair of (0, n) which is the only pair changing A
    - we shrink A each loop by 2^n
    - the l-th digit changes every (2^n)^(l-1)-steps (appears in the first)
    - check all start pos candidates where the l-th last digit is the same as the l-th last from target
    - if not identical, increment start pos by (2^n)^(l-1)
    - iterate between l = size(input)-1 and 1
    */

    int64_t size_output = instructions.size();
    uint16_t factor = 0;
    for(uint8_t n = 0; n < size_output; n+=2){
        if(instructions[n] == 0) factor = pow(2, instructions[n + 1]);
    }
    typedef std::pair<int64_t, int64_t> start_cdt;
    std::deque<start_cdt> start_A_pos_cdt = {start_cdt(0, size_output - 1)}; //0, 15 for real input
    while(!start_A_pos_cdt.empty()){
        auto cdt_a = start_A_pos_cdt.back();
        auto start_A = cdt_a.first;
        for(int i = 0; i < 8; i++){
            std::string curr_solve = solve_code(instructions, start_A, B, C,"");
            int64_t last = static_cast<int64_t>(curr_solve[cdt_a.second]) - '0';
            if(last == instructions[cdt_a.second]) {
                if(cdt_a.second > 0) start_A_pos_cdt.push_front(start_cdt(start_A, cdt_a.second -1));
                else if(start_A < res.second) res.second = start_A;
            }
            start_A += pow(factor, cdt_a.second);
        }
        start_A_pos_cdt.pop_back();
    }
    
    return res;
}

int main(){
    std::pair<std::string, int64_t> res;
    res = solve_puzzle("inputs/Test_17.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    res = solve_puzzle("inputs/Test_17_2.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_17.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}