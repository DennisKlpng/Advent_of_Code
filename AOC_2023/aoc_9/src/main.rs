use rust_utils as utils;

fn get_extrapolation(history: &Vec<i64>) -> i64{
    let mut current_vec: Vec<_> = history.clone();
    let mut last_elems: Vec<i64> = vec![];
    while !current_vec.iter().all(|&x| x == 0) && current_vec.len() > 0{
        // println!("{:?}", current_vec);
        last_elems.push(*current_vec.last().unwrap_or(&0i64));
        current_vec = current_vec.windows(2).map(|sl| sl[1]-sl[0]).collect::<Vec<i64>>();
    }
    last_elems.iter().sum()
}

fn solve(filename: &str) -> (i64, u64){
    let mut res = (0i64, 0u64);
    if let Ok(lines) = utils::file_read::read_file_as_lines(filename){
        for line in lines {
            res.0 += get_extrapolation(&utils::string_utils::split_string_i64(&line));
        }
    };

    res
}

fn main(){
    let res = solve("../inputs/Test_9.txt");
    println!("Test res pt 1: {:?} pt2: {:?}", res.0, res.1);
    use std::time::Instant;
    let now = Instant::now();
    let res = solve("../inputs/Data_9.txt");
    let elapsed = now.elapsed();
    println!("Puzzle res: pt 1: {:?} pt2: {:?} execution time: {:.2?}", res.0, res.1, elapsed);

}