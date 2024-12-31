use rust_utils as utils;
use grid::Grid;
use regex::Regex;

struct GridNumber{
    min_x: usize,
    max_x: usize,
    y: usize,
    num: u64
}

fn get_diag_neighbours(pt: (usize, usize)) -> Vec<(usize, usize)>{
    
    let res: Vec<(usize, usize)> = vec![(pt.0 - 1usize, pt.1 - 1usize),
                                        (pt.0 - 1usize, pt.1),
                                        (pt.0 - 1usize, pt.1 + 1usize),
                                        (pt.0, pt.1 - 1usize),
                                        (pt.0, pt.1 + 1usize),
                                        (pt.0 + 1usize, pt.1 - 1usize),
                                        (pt.0 + 1usize, pt.1),
                                        (pt.0 + 1usize, pt.1 + 1usize)];
    res
}

fn solve(filename: &str) -> (u64, u64){
    let mut res = (0u64, 0u64);
    let mut grid_nums: Vec<GridNumber> = vec![];
    let mut grid : Grid<char>;
    let re = Regex::new(r"[0-9]+").unwrap();
    if let Ok(lines) = utils::file_read::read_file_as_lines(filename){
        //pad grid on each side
        grid = Grid::init(lines.len() + 2, lines[0].len() + 2, '.');
        for (line, y) in lines.into_iter().zip(1usize..) {
            for (c, x) in line.chars().zip(1usize..){
                grid[(y, x)] = c;
            }
            for num in re.find_iter(line.as_str()){
                //+1 on min since we padded our grid with 1, not necessary for max since end returns the byte behind the match
                let gn = GridNumber{min_x: num.start() + 1, max_x: num.end(), y, 
                                                num: num.as_str().parse().unwrap()};
                grid_nums.push(gn);
            }
        }
        for gn in grid_nums.iter(){
            let mut adj = false;
            //search all neighbours if there's a symbol there
            for x in gn.min_x..=gn.max_x{
                //check all neighbours
                if get_diag_neighbours((gn.y, x)).iter().any(
                    |x| grid[*x] != '.' && !grid[*x].is_ascii_digit()){
                    adj = true;
                }
            }
            if adj{
                res.0 += gn.num;
            }
        }
    };
    res
}

fn main(){
    let res = solve("../inputs/Test_3.txt");
    println!("Test res pt 1: {:?} pt2: {:?}", res.0, res.1);
    use std::time::Instant;
    let now = Instant::now();
    let res = solve("../inputs/Data_3.txt");
    let elapsed = now.elapsed();
    println!("Puzzle res: pt 1: {:?} pt2: {:?} execution time: {:.2?}", res.0, res.1, elapsed);
}