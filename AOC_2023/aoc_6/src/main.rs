use std::iter::zip;

use rust_utils as utils;

fn get_num_dist(params: (i64, i64)) -> u64{
    //+- 0.0001 to prevent matching when exactly equal
    let min = (0.5 * params.0 as f64 - 0.5*((params.0*params.0 - 4* params.1) as f64).sqrt() + 0.0001).ceil();
    let max = (0.5 * params.0 as f64 + 0.5*((params.0*params.0 - 4* params.1) as f64).sqrt() - 0.0001).floor();
    (max - min + 1f64) as u64
}

fn solve(filename: &str) -> (u64, u64){
    let mut res = (1u64, 0u64);
    if let Ok(lines) = utils::file_read::read_file_as_lines(filename){
        let times = utils::string_utils::split_string_i64(lines.get(0).unwrap());
        let distances = utils::string_utils::split_string_i64(lines.get(1).unwrap());
        let (mut time_2, mut dist_2) = ("".to_owned(), "".to_owned());
        for game in zip(times, distances){
            res.0 *= get_num_dist(game);
            time_2 += &game.0.to_string();
            dist_2 += &game.1.to_string();
        }
        res.1 = get_num_dist((time_2.parse().unwrap(), dist_2.parse().unwrap()));
    };

    res
}

fn main(){
    let res = solve("../inputs/Test_6.txt");
    println!("Test res pt 1: {:?} pt2: {:?}", res.0, res.1);
    assert!(res.0 == 288);
    assert!(res.1 == 71503);
    use std::time::Instant;
    let now = Instant::now();
    let res = solve("../inputs/Data_6.txt");
    let elapsed = now.elapsed();
    println!("Puzzle res: pt 1: {:?} pt2: {:?} execution time: {:.2?}", res.0, res.1, elapsed);

}