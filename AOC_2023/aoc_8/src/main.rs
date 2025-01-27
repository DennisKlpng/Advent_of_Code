use rust_utils as utils;
use rustc_hash::FxHashMap;
use num::integer::lcm;

fn iterate(inst: &String, map: &FxHashMap<String, (String, String)>, start: &String, pt2: bool) -> u64{
    let mut pos = start;
    let mut index_instr = 0;
    let mut ret_val = 0u64;
    loop {
        let map_val = map.get(pos).unwrap();
        if inst.chars().nth(index_instr) == Some('L'){
            pos = &map_val.0;
        }
        else{
            pos = &map_val.1;
        }
        ret_val += 1;
        //for pt2 it loops with the num of steps until the first occurence of an end
        if (pos == "ZZZ" && !pt2) | (pos.chars().last().unwrap() == 'Z' && pt2){
            return ret_val
        }
        index_instr += 1;
        index_instr %= inst.len();
    }
}

fn solve(filename: &str, pt2: bool) -> u64{
    let mut res = 0u64;
    let mut map: FxHashMap<String, (String, String)> = FxHashMap::default();
    let mut starting_pts_2: Vec<String> = vec![];
    if let Ok(blocks) = utils::file_read::read_file_block_lines(filename){
        let inst = blocks.first().unwrap().first().unwrap();
        for line in blocks.get(1).unwrap() {
            let mut dir = line.to_owned();
            dir = dir.replace(['=', '(', ')', ','], "");
            let mut iter = dir.split(" ").peekable();
            if iter.peek().unwrap().chars().last().unwrap() == 'A'{
                starting_pts_2.push(iter.peek().unwrap().to_string());
            }
            map.insert(iter.peek().unwrap().to_string(), 
                (iter.nth(2).unwrap().to_owned(), iter.next().unwrap().to_owned()));
        }
        if ! pt2{
            res = iterate(&inst, &map, &String::from("AAA"), pt2);
        }
        else{
            //calculate least common multiple of all step-sizes needed to loop
            //for this: calc lcm in each step
            res = starting_pts_2.into_iter().fold(1, 
                |acc, start| lcm(acc, iterate(&inst, &map, &start, pt2))
            );
        }
    };

    res
}

fn main(){
    let res1 = solve("../inputs/Test_8.txt", false);
    let res2 = solve("../inputs/Test_8_pt2.txt", true);
    println!("Test res pt 1: {:?} pt2: {:?}", res1, res2);
    use std::time::Instant;
    let now = Instant::now();
    let res1 = solve("../inputs/Data_8.txt", false);
    let res2 = solve("../inputs/Data_8.txt", true);
    let elapsed = now.elapsed();
    println!("Puzzle res: pt 1: {:?} pt2: {:?} execution time: {:.2?}", res1, res2, elapsed);

}