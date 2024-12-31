use rust_utils as utils;

fn get_nums(line: &String, pt2: bool) -> u64{
    let mut line_loc:String = line.to_owned();
    if pt2{ //adapt string before analyzing
        let nums = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine"];
        for (i, num) in nums.iter().enumerate(){
            //get all matches for current number
            let v: Vec<_> = line_loc.match_indices(num).map(|x| x.0).collect();
            for c in v{
                //replace second character in text number with digit number => also works with overlaps
                line_loc.replace_range(c+1..c+2, char::from_digit(i as u32 + 1, 10).unwrap().to_string().as_str());
            }
        }
    }
    
    // idea: filter on success of converting each char in string to digit, then convert back to string and
    // concat strings into common string and convert to int (unwrap since first/last returns optional and parse returns optional)
    let digits:Vec<String> = line_loc.chars().filter_map(|c| c.to_digit(10)).map(|x| x.to_string()).collect();
    let val:u64 = format!("{}{}", digits.first().unwrap(), digits.last().unwrap()).parse().unwrap();
    val
}

fn solve(filename: &str) -> (u64, u64){
    let mut res = (0u64, 0u64);
    if let Ok(lines) = utils::file_read::read_file_as_lines(filename){
        for line in lines {
            // println!("{}", line);
            res.0 += get_nums(&line, false);
            res.1 += get_nums(&line, true);
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