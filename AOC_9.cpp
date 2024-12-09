#include "utils.hpp"
#include <deque>
#include <queue>
#include <cmath>

uint64_t solve_part(std::vector<uint8_t> input_queue,
                    std::map<uint8_t, std::deque<std::uint64_t>> freespace_queue,
                    uint64_t back_index_file, std::vector<uint64_t>& vec_startindex, bool pt2 = false){
    std::vector<uint64_t> vec_tgt_pos(vec_startindex.back(), 0); //isn't guaranteed to be big enough, but will be for our inputs
    int8_t back_size_remaining = 0;
    while(!input_queue.empty()){
        uint64_t index_file_start = vec_startindex[back_index_file];
        back_size_remaining = input_queue.back();
        int8_t space_used = back_size_remaining;
        input_queue.pop_back();
        uint64_t min_index = std::numeric_limits<uint64_t>::max();
        int8_t size_index = 0;
        //search all freespaces for the one with the minimal pos index (aka nearest to front) where stuff fits
        for(auto &[k, v]: freespace_queue){
            if(v.empty()) continue;
            if(pt2 && k < back_size_remaining) continue;
            if(v.front() < min_index){
                min_index = v.front();
                size_index = k;
            }
        }
        if(min_index > index_file_start){
            //we can't move this file forward => go to next file and enter it at it's current position
            for(int i = 0; i < back_size_remaining; i++){
                vec_tgt_pos.at(index_file_start + i) = back_index_file;
            }
            back_index_file--;
            continue;
        }
        back_size_remaining -= size_index;
        //remove the free space we are filling
        freespace_queue[size_index].pop_front();
        //the stuff we just moved needs to be stored in the target
        space_used = std::min(space_used, size_index);
        for(int i = 0; i < space_used; i++){
            vec_tgt_pos.at(min_index + i) = back_index_file;
        }

        if(back_size_remaining > 0){
            //there is still a part of the file left, can only occur in pt 1 => stuff the rest back into the input queue
            input_queue.push_back(back_size_remaining);
        }
        else{
            if(back_size_remaining < 0){
                //we used LESS space than was available in that space => store the remaining space
                if(freespace_queue.contains(-back_size_remaining)){
                    auto& deq = freespace_queue[-back_size_remaining];
                    deq.insert(std::ranges::upper_bound(deq, min_index + space_used), min_index + space_used);
                }
                else{
                    freespace_queue.emplace(-back_size_remaining, std::deque<std::uint64_t>{min_index + space_used});
                }
            }
            //move to next file from the back
            back_index_file--;
        }
    }
    uint64_t res = 0;
    for(uint64_t i = 0; i < vec_tgt_pos.size(); i++){
        res += i*vec_tgt_pos[i];
    }
    return res;
}

std::pair<uint64_t, uint64_t> solve_puzzle(std::string const& filename){
    std::pair<uint64_t, uint64_t> res{0, 0};
    std::string input = read_file_as_lines(filename)[0];
    std::vector<uint8_t> input_queue;
    std::vector<uint64_t> vec_startindex;
    std::map<uint8_t, std::deque<std::uint64_t>> freespace_queue;
    bool next_is_file = true;
    uint64_t front_index_file = 0;

    //filling queues => precalc
    for(auto const& c: input) {
        auto num = static_cast<uint8_t>(std::stoi(std::string(1, c)));
        if(next_is_file){
            input_queue.push_back(num);
            vec_startindex.push_back(front_index_file);
            next_is_file = false;
        }
        else{
            next_is_file = true;
            if(num == 0) continue;
            if(!freespace_queue.try_emplace(num, std::deque<std::uint64_t>{front_index_file}).second){
                freespace_queue[num].push_back(front_index_file);
            }
        }
        front_index_file += num;
    }

    uint64_t back_index_file = input_queue.size() - 1;
    res.first = solve_part(input_queue, freespace_queue, back_index_file, vec_startindex, false);
    res.second = solve_part(input_queue, freespace_queue, back_index_file, vec_startindex, true);
    return res;
}

int main(){
    std::pair<uint64_t, uint64_t> res = solve_puzzle("inputs/Test_9.txt");
    print("Test res pt 1: ", res.first, " pt 2: ", res.second, "\n");
    double time_spent;
    res = profile_function(solve_puzzle, time_spent, "inputs/Data_9.txt");
    print("Puzzle res pt 1: ", res.first, " pt 2: ", res.second, " puzzle calculation took: ", time_spent, " ms \n");

    return 0;
}