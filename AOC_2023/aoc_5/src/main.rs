use rust_utils as utils;

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

fn get_min_pos(seeds: &Vec<i64>, mappings: &Vec<Vec<Mapping>>) -> u64{
    let mut pos: Vec<i64> = vec![];
    for seed in seeds{
        //iter over vector of mappings
        //in each step we fold a mapping from the almanach and use the new value for next step:
        //seed => (transform with seed2soil mapping) => soil => ... => position
        pos.push(mappings.iter()
            .fold(*seed, |acc, x| fold_level(x, acc)));
    }
    *pos.iter().min().unwrap() as u64
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
        res.0 = get_min_pos(&seeds, &mappings);
        //extend seeds by ranges: zip iter getting all starts (index 0, 2, ..) with iter getting all ends (1, 3, ..)
        //basically we create a loop here that uses i and i+1 in every step and iterates i by 2
        //then we append the whole range start..end=start+length (exclusive) to the new seeds list
        let mut seeds_ext: Vec<i64> = vec![];
        for (a, b) in seeds.iter().step_by(2).zip(seeds.iter().skip(1).step_by(2)){
            seeds_ext.append(&mut (*a..(*a + *b)).collect::<Vec<i64>>());
        }
        res.1 = get_min_pos(&seeds_ext, &mappings);
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