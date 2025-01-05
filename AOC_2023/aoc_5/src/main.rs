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

fn solve(filename: &str) -> (u64, u64){
    let mut res = (0u64, 0u64);
    let mut mappings: Vec<Vec<Mapping>> = vec![];
    if let Ok(blocks) = utils::file_read::read_file_block_lines(filename){
        let seeds = utils::string_utils::split_string_i64(blocks.first().unwrap().first().unwrap());
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
        let mut pos: Vec<i64> = vec![];
        for seed in seeds{
            pos.push(mappings.iter()
               .fold(seed, |acc, x| fold_level(&x, acc)));
        }
        res.0 = *pos.iter().min().unwrap() as u64;
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