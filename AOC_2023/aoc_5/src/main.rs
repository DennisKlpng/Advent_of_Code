use rust_utils as utils;
use std::ops::Range;

struct Mapping{
    min_source: i64,
    max_source: i64,
    offset: i64
}

fn fold_level(lvl_mapping: &Vec<Mapping>, num: i64) -> i64{
    for mapping in lvl_mapping{
        if mapping.min_source <= num && num <= mapping.max_source{
            return num + mapping.offset;
        }
    }
    num
}

fn fold_level_single_range(lvl_mapping: &Vec<Mapping>, mut range: Range<i64>) -> Vec<Range<i64>>{
    let mut res = vec![];
    for mapping in lvl_mapping{
        if range.is_empty() {return res;} //early exit
        let (map_min, map_max, map_offset) = (mapping.min_source, mapping.max_source, mapping.offset);
        let last_elem = range.end - 1;
        //we have different cases:
        //input is completely contained in the mapping range
        if map_min <= range.start && map_max >= last_elem{
            res.push(range.start + map_offset .. range.end + map_offset);
            range.start = range.end; //we have nothing more to analyze
        }
        //start of range is before the mapping, but end is in mapping => split
        else if range.start < map_min && range.end >= map_min && map_max >= last_elem{
            res.push(map_min + map_offset .. range.end + map_offset);
            range.end = map_min; //no +1 since range.end is past the range => map_min is the first element that isn't in range
        }
        //start of range is in mapping, but end is outside => split
        else if map_min <= range.start && range.start <= map_max && last_elem > map_max{
            res.push(range.start + map_offset .. map_max + 1 + map_offset); //+ 1 since end of range behind
            range.start = map_max + 1; //+1 since the first element is the next one after the almanach range
        }
        //start of range is before mapping and end of range is behind mapping => split in 3 parts
        else if range.start < map_min && last_elem > map_max{
            //part 3: recursively calc everything behind the mapping (do first because range.end is changed by part 1)
            res.append(&mut fold_level_single_range(lvl_mapping, map_max + 1 .. range.end));
            //part 1: before the mapping => remains
            range.end = map_min;
            //part 2: in the mapping => map
            res.push(map_min + map_offset .. map_max + 1 + map_offset);
        }
    }
    //only reached when we couldn't map the (remaining) range
    res.push(range);
    res
}

fn get_min_pos_pt1(seeds: &Vec<i64>, mappings: &Vec<Vec<Mapping>>) -> u64{
    let mut pos= vec![];
    for seed in seeds{
        //iter over vector of mappings
        //in each step we fold a mapping from the almanach and use the new value for next step:
        //seed => (transform with seed2soil mapping) => soil => ... => position
        pos.push(mappings.iter()
            .fold(*seed, |acc, x| fold_level(x, acc)));
    }
    *pos.iter().min().unwrap() as u64
}

fn get_min_pos_pt2(seeds: &Vec<i64>, mappings: &Vec<Vec<Mapping>>) -> u64{
    //store ranges of seeds: zip iter getting all starts (index 0, 2, ..) with iter getting all lengths (1, 3, ..)
    //basically we create a loop here that uses i and i+1 in every step and iterates i by 2
    //then we push the whole range start..end=start+length to the new seeds list
    let mut curr_ranges = vec![];
    for (a, b) in seeds.iter().step_by(2).zip(seeds.iter().skip(1).step_by(2)){
        curr_ranges.push(*a..(*a + *b));
    }

    //now fold similarly to part 1, but with ranges instead of individual seeds
    let pos_range = mappings.iter()
        .fold(curr_ranges, |curr_ranges, x| {
            //each step: translate all ranges, flatten them into a single vector of ranges
            curr_ranges
            .into_iter()
            .flat_map(|range| fold_level_single_range(x, range))
            .collect()
    });

    pos_range.iter().map(|x| x.start).min().unwrap() as u64
}

fn solve(filename: &str) -> (u64, u64){
    let mut res = (0u64, 0u64);
    let mut mappings: Vec<Vec<Mapping>> = vec![];
    if let Ok(blocks) = utils::file_read::read_file_block_lines(filename){
        //the first block is the list of seeds
        let seeds = utils::string_utils::split_string_i64(blocks.first().unwrap().first().unwrap());
        //every block after the first is a mapping
        for block in blocks.iter().skip(1) {
            let mut type_mapping: Vec<Mapping> = vec![];
            for line in block{
                if line.chars().next().unwrap().is_ascii_alphabetic() {
                    continue;
                }
                let nums = utils::string_utils::split_string_i64(line);
                let map = Mapping{min_source: nums[1], max_source: nums[1] + nums[2] - 1, 
                                           offset: nums[0] - nums[1]};
                type_mapping.push(map);
            }
            mappings.push(type_mapping);
        }
        res.0 = get_min_pos_pt1(&seeds, &mappings);
        res.1 = get_min_pos_pt2(&seeds, &mappings);
    };

    res
}

fn main(){
    let res = solve("../inputs/Test_5.txt");
    println!("Test res pt 1: {:?} pt2: {:?}", res.0, res.1);
    use std::time::Instant;
    let now = Instant::now();
    let res = solve("../inputs/Data_5.txt");
    let elapsed = now.elapsed();
    println!("Puzzle res: pt 1: {:?} pt2: {:?} execution time: {:.2?}", res.0, res.1, elapsed);
}