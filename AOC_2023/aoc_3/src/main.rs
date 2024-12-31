use std::collections::HashSet;

use rust_utils as utils;
use grid::Grid;
use regex::Regex;
use rustc_hash::FxHashMap;

struct GridNumber{
    min_x: usize,
    max_x: usize,
    y: usize,
    num: u64
}

fn get_diag_neighbours_num(num: &GridNumber) -> Vec<(usize, usize)>{
    let mut res:HashSet<(usize, usize)> = HashSet::new();
    for x in num.min_x..=num.max_x{
        let pt: (usize, usize) = (num.y, x);
        res.insert((pt.0 - 1usize, pt.1 - 1usize));
        res.insert((pt.0 - 1usize, pt.1));
        res.insert((pt.0 - 1usize, pt.1 + 1usize));
        res.insert((pt.0, pt.1 - 1usize));
        res.insert((pt.0, pt.1 + 1usize));
        res.insert((pt.0 + 1usize, pt.1 - 1usize));
        res.insert((pt.0 + 1usize, pt.1));
        res.insert((pt.0 + 1usize, pt.1 + 1usize));
    }

    return res.into_iter().collect();
}

fn solve(filename: &str) -> (u64, u64){
    let mut res = (0u64, 0u64);
    let mut grid_nums: Vec<GridNumber> = vec![];
    let mut grid : Grid<char>;
    let mut gears: FxHashMap<(usize, usize), Vec<u64>> = FxHashMap::default();
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
            //search all neighbours => we have multiple x-values available!
            for nb in get_diag_neighbours_num(gn){
                if grid[nb] != '.' && !grid[nb].is_ascii_digit(){
                    adj = true;
                }
                if grid[nb] == '*'{
                    let v = gears.entry(nb).or_default();
                    v.push(gn.num);
                }
            }
            if adj{
                res.0 += gn.num;
            }
        }
        for (_gear, nb_nums) in gears{
            if nb_nums.len() == 2{
                //save since length is 2
                res.1 += nb_nums.into_iter().fold(1, |prod, a| prod * a);
            }
        }
    }
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