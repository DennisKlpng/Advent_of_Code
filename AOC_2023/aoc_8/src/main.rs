use rust_utils as utils;
use rustc_hash::FxHashMap;

fn solve(filename: &str, pt2: bool) -> u64{
    let mut res = 0u64;
    let mut map: FxHashMap<String, (String, String)> = FxHashMap::default();
    if let Ok(blocks) = utils::file_read::read_file_block_lines(filename){
        let inst = blocks.first().unwrap().first().unwrap();
        for line in blocks.get(1).unwrap() {
            let mut dir = line.to_owned();
            dir = dir.replace(['=', '(', ')', ','], "");
            let mut iter = dir.split(" ");
            map.insert(iter.next().unwrap().to_owned(), 
                (iter.nth(1).unwrap().to_owned(), iter.next().unwrap().to_owned()));
        }
        if ! pt2{
            let mut pos = "AAA";
            let mut index_instr = 0;
            while pos != "ZZZ"{
                let map_val = map.get(pos).unwrap();
                if inst.chars().nth(index_instr) == Some('L'){
                    pos = &map_val.0;
                }
                else{
                    pos = &map_val.1;
                }
                index_instr += 1;
                res += 1;
                index_instr %= inst.len();
            }
        }
        else{

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