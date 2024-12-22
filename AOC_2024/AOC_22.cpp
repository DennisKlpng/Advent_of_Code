#include "utils.hpp"

int64_t hash_seq(const std::vector<int8_t>& vec){
    return 10 + vec[0] /*1-19*/ + (10 + vec[1]) * 20 + (10 + vec[2]) * 20*20 + (10 + vec[3]) * 20*20*20;  
}

uint64_t calc_secret_num(const uint64_t input, const uint64_t steps, std::map<int64_t, int8_t>& banana_map){
    //steps, after each mix + prune
    //multiply by 64 => 2^6 => shift left by 6 bits
    //divide by 32 => 2^5 => shift right by 5 bits
    //multiply by 2048 => 2^11 => shift left by 11 bits
    //mix: a XOR b (bitwise)
    //prune: a mod b with b = 2^24 => only take the rightmost 24 bits
    uint64_t secret = input;
    std::vector<int8_t> last_pricediffs{0, 0, 0, 0};
    int8_t last_price = 0;
    
    for ([[maybe_unused]] uint64_t i : std::views::iota(1u, steps + 1)){
        secret = (secret << 6) ^ secret;
        secret = secret & 0b111111111111111111111111;

        secret = (secret >> 5) ^ secret;
        secret = secret & 0b111111111111111111111111;

        secret = (secret << 11) ^ secret;
        secret = secret & 0b111111111111111111111111;

        int8_t price = secret % 10;
        std::rotate(last_pricediffs.rbegin(), last_pricediffs.rbegin() + 1, last_pricediffs.rend());
        last_pricediffs[0] = price - last_price;
        last_price = price;
        if(i > 4u){
            int64_t sequence_hash = hash_seq(last_pricediffs);
            if(!banana_map.contains(sequence_hash)) banana_map.emplace(sequence_hash, price);
        }
    }

    return secret;
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string filename){
    std::pair<uint64_t, uint64_t> res{0, 0};

    auto data = read_file_as_lines(filename);
    std::map<int64_t, int64_t> map_totals;

    for(auto& line: data){
        auto start_num = get_ints_from_string(line)[0];
        std::map<int64_t, int8_t> map_buyer;
        res.first += calc_secret_num(start_num, 2000, map_buyer);
        for(auto& [k, v]: map_buyer){
            if(!map_totals.contains(k)) map_totals.emplace(k, v);
            else map_totals[k] += v;
        }
    }
    auto vals = std::views::values(map_totals);
    res.second = *std::max_element(vals.begin(), vals.end());

    return res;
}

void test() {
    uint64_t start = 123;
    for ([[maybe_unused]] uint64_t i : std::views::iota(1, 10)){
        std::map<int64_t, int8_t> map_buyer;
        start = calc_secret_num(start, 1, map_buyer);
        print(start);
    }

}

int main(){
    // test();
    // std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_22.txt");
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_22_2.txt");
    print("Test res pt 1:", res.first, "pt 2:", res.second);
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_22.txt");
    print("Puzzle res pt 1:", res.first, "pt 2:", res.second, "puzzle calculation took:", time_spent, "ms");

    return 0;
}