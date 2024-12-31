use std::collections::HashSet;

use rust_utils as utils;

fn solve(filename: &str) -> (u64, u64){
    let mut res: (u64, u64) = (0u64, 0u64);
    if let Ok(lines) = utils::file_read::read_file_as_lines(filename){
        //preallocate vector, since we have contiguous game nums
        let mut cards:Vec<u64> = vec![1; lines.len() + 1];
        cards[0] = 0; //card with index 0 doesn't exist, since games start at 1
        for (line, index) in lines.into_iter().zip(1usize..) {
            let (l, r) = line.split_once(":").unwrap().1.split_once("|").unwrap();
            let num_card = HashSet::<u64>::from_iter(
                utils::string_utils::split_string_u64(l));
            let num_wins = HashSet::<u64>::from_iter(
                utils::string_utils::split_string_u64(r));
            let intersect:HashSet<_> = num_card.intersection(&num_wins).collect();
            
            //we don't have to check for existence here, since all card indices do exist before
            let num_curr_card = cards[index];
            if intersect.len() > 0{
                let base: u64 = 2;
                res.0 += base.pow(intersect.len() as u32 - 1);
                //add numbers to card vector => since it always exists just add the number
                for i in index as usize + 1 ..= index as usize + intersect.len(){
                    if let Some(x) = cards.get_mut(i){
                        *x += num_curr_card;
                    }
                }
            }
        }
        res.1 = cards.iter().sum();
    }

    res
}

fn main(){
    let res = solve("../inputs/Test_4.txt");
    println!("Test res pt 1: {:?} pt2: {:?}", res.0, res.1);
    use std::time::Instant;
    let now = Instant::now();
    let res = solve("../inputs/Data_4.txt");
    let elapsed = now.elapsed();
    println!("Puzzle res: pt 1: {:?} pt2: {:?} execution time: {:.2?}", res.0, res.1, elapsed);

}