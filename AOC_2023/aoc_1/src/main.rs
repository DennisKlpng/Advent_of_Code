use rust_utils as utils;
use regex::Regex;

fn solve(filename: &str) -> (u64, u64){
    let mut res = (0u64, 0u64);
    if let Ok(lines) = utils::file_read::read_file_as_lines(filename){
        for line in lines.flatten() {
            // println!("{}", line);
            let re = Regex::new(r"[0-9]").unwrap();
            let matches:Vec<&str> = re.find_iter(&line).map(|x| x.as_str()).collect();
            // concat strings into common string and convert to int (unwrap since first/last returns optional)
            let val:u64 = format!("{}{}", matches.first().unwrap(), matches.last().unwrap()).parse().unwrap();
            // println!("{}", val);
            res.0 += val;
        }
    };

    res
}

fn main(){
    let res = solve("../inputs/Test_1.txt");
    println!("Test res pt 1: {:?} pt2: {:?}", res.0, res.1);
    use std::time::Instant;
    let now = Instant::now();
    let res = solve("../inputs/Data_1.txt");
    let elapsed = now.elapsed();
    println!("Puzzle res: pt 1: {:?} pt2: {:?} execution time: {:.2?}", res.0, res.1, elapsed);

}