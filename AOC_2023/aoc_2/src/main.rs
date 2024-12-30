use rustc_hash::FxHashMap;
use rust_utils as utils;

fn comp_set(curr_set: &str, colors_max: &FxHashMap<&str, i32>, colors_min: &mut FxHashMap<&str, i32>) -> bool{
    let mut res = true;
    let mut it = curr_set.split_ascii_whitespace();
    //iterate over splitted string, in each pass we iterate twice (for num and color)
    //check if set is valid and accumulate min number of dices at the same time
    while let Some(x) = it.next(){
        let num_dices = x.parse().unwrap();
        let color = it.next().unwrap().trim_end_matches(",");
        if num_dices > colors_max[color]{
            res = false;
        }
        if num_dices > colors_min[color]{
            *colors_min.get_mut(color).unwrap() = num_dices;
        }
    }    
    res
}

fn solve(filename: &str) -> (u64, u64){
    let mut res = (0u64, 0u64);
    let colors_max: FxHashMap<&str, i32> = [
            ("red", 12),
            ("green", 13),
            ("blue", 14),].into_iter().collect();
    let colors_min: FxHashMap<&str, i32> = [
            ("red", 0),
            ("green", 0),
            ("blue", 0),].into_iter().collect();
    if let Ok(lines) = utils::file_read::read_file_as_lines(filename){
        for (i, line) in lines.flatten().into_iter().enumerate() {
            let mut colors_min_loc = colors_min.to_owned();
            let sets: Vec<_> = line.split(":").last().unwrap().split(";").map(|x| x.trim()).collect();
            //map from set to validity, does the part 2 accumulation (get min num dices for each game in colors_min_loc) on the fly
            let set_res: Vec<bool> = sets.iter().map(|x| comp_set(x, &colors_max, &mut colors_min_loc)).collect();
            if set_res.into_iter().all(|x| x){
                res.0 += i as u64 + 1;
            }
            //get power of game by multiplying all dice numbers and add to res
            res.1 += colors_min_loc.values().cloned().collect::<Vec<i32>>().iter().fold(1, |prod, a| prod * a) as u64;
        }
    };

    res
}

fn main(){
    let res = solve("../inputs/Test_2.txt");
    println!("Test res pt 1: {:?} pt2: {:?}", res.0, res.1);
    use std::time::Instant;
    let now = Instant::now();
    let res = solve("../inputs/Data_2.txt");
    let elapsed = now.elapsed();
    println!("Puzzle res: pt 1: {:?} pt2: {:?} execution time: {:.2?}", res.0, res.1, elapsed);
}