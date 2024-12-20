#include "utils.hpp"
#include <complex>
#include <numeric>

inline bool check_in_range(int16_t val, int16_t min, int16_t max){
    if(val >= min && val <= max) return true;
    return false;
}

inline std::complex<int16_t> get_elementary_vector(std::complex<int16_t> in){
    //the elementar vector on the grid is the vector that still has ints as both coordinates
    //which is the smallest fraction of the input
    //=> divide both coords by some int resulting in ints => greatest common divisor
    auto divisor = std::gcd(in.real(), in.imag());
    return std::complex<int16_t>(in.real()/divisor, in.imag()/divisor);
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};
    std::map<char, std::vector<std::complex<int16_t>>> map_antennas;
    std::vector<std::string> lines = read_file_as_lines(filename);
    int16_t max_x = lines[0].size();
    int16_t max_y = lines.size();
    int16_t y = max_y;
    for(auto& line: lines){
        int16_t x = 1;
        for(auto& c : line){
            if (c == '.') {x++; continue;}
            if (!map_antennas.try_emplace(c, std::vector<std::complex<int16_t>>{std::complex<int16_t>(x, y)}).second){
                map_antennas.at(c).push_back(std::complex<int16_t>(x, y));
            }
            x++;
        }
        y--;
    }
    std::unordered_set<int16_t> antinodes_1;
    std::unordered_set<int16_t> antinodes_2;
    for(auto& [k, v]: map_antennas){
        for(auto it1 = v.begin(); it1 != v.end(); it1++){
            for(auto it2 = it1+1; it2 != v.end(); it2++){
                auto delta = *it2 - *it1;
                //part 1: simply calculate the two candidates: vector between antenna 2 and 1 added to each side
                auto c1 = *it2 + delta;
                //hashing the complex number to an int, since c++ doesn't allow complex numbers in sets 
                //(unless you implement your own operators for unordered_set)
                if(check_in_range(c1.real(), 1, max_x) && check_in_range(c1.imag(), 1, max_y)) antinodes_1.emplace(c1.real()*1000 + c1.imag());
                auto c2 = *it1 - delta;
                if(check_in_range(c2.real(), 1, max_x) && check_in_range(c2.imag(), 1, max_y)) antinodes_1.emplace(c2.real()*1000 + c2.imag());
                //part 2: normalize the vector between the two antennas to the elementary vector for that connection
                auto delta_min = get_elementary_vector(delta);
                //iterate forwards from antenna 1
                auto c_test = *it1;
                while(check_in_range(c_test.real(), 1, max_x) && check_in_range(c_test.imag(), 1, max_y)){
                    antinodes_2.emplace(c_test.real()*1000 + c_test.imag());
                    c_test += delta_min;
                }
                //iterate backwards from antenna 1
                c_test = *it1 - delta_min;
                while(check_in_range(c_test.real(), 1, max_x) && check_in_range(c_test.imag(), 1, max_y)){
                    antinodes_2.emplace(c_test.real()*1000 + c_test.imag());
                    c_test -= delta_min;
                }
            }
        }
    }
    res.first = antinodes_1.size();
    res.second = antinodes_2.size();
    

    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_8.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_8.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}