use std::iter::zip;

use rust_utils as utils;

fn solve(filename: &str) -> (u64, u64){
    let mut res = (1u64, 0u64);
    if let Ok(lines) = utils::file_read::read_file_as_lines(filename){
        let times = utils::string_utils::split_string_i64(lines.get(0).unwrap());
        let distances = utils::string_utils::split_string_i64(lines.get(1).unwrap());
        for game in zip(times, distances){
            //+- 0.0001 to prevent matching when exactly equal
            let min = (0.5 * game.0 as f64 - 0.5*((game.0*game.0 - 4* game.1) as f64).sqrt() + 0.0001).ceil();
            let max = (0.5 * game.0 as f64 + 0.5*((game.0*game.0 - 4* game.1) as f64).sqrt() - 0.0001).floor();
            res.0 *= (max - min + 1f64) as u64;
        }
    };

    res
}

fn main(){
    let res = solve("../inputs/Test_6.txt");
    println!("Test res pt 1: {:?} pt2: {:?}", res.0, res.1);
    use std::time::Instant;
    let now = Instant::now();
    let res = solve("../inputs/Data_6.txt");
    let elapsed = now.elapsed();
    println!("Puzzle res: pt 1: {:?} pt2: {:?} execution time: {:.2?}", res.0, res.1, elapsed);

}