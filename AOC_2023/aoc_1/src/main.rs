use rust_utils as utils;

fn solve(filename: &str) -> (u64, u64){
    let res = (0u64, 0u64);
    if let Ok(lines) = utils::file_read::read_file_as_lines(filename){
        for line in lines.flatten() {
            // println!("{}", line);
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