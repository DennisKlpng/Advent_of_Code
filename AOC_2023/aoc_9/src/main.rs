use rust_utils as utils;

fn get_extrapolation(history: &Vec<i64>) -> (i64, i64){
    let mut current_vec: Vec<_> = history.clone();
    let mut last_elems: Vec<i64> = vec![];
    let mut first_elems: Vec<i64> = vec![];
    while !current_vec.iter().all(|&x| x == 0) && current_vec.len() > 0{
        // println!("{:?}", current_vec);
        last_elems.push(*current_vec.last().unwrap_or(&0i64));
        first_elems.push(*current_vec.first().unwrap_or(&0i64));
        current_vec = current_vec.windows(2).map(|sl| sl[1]-sl[0]).collect::<Vec<i64>>();
    }
    
    // let x_i_j denote the i_th element on the j-th layer from the top
    // for x_i_j and x_k_j, with i+1 = k, the resulting element on the next layer is r = x_k_j - x_i_j 
    //=> from this we can calc both x_i_j and x_k_j if we have r and the other
    // with this we get for the last (m-th) element, when 0 is the top layer:
    // x_m_0 = x_m-1_0 + x_m_1 = x_m-1_0 + (x_m-1_1 + x_m_2) = sum_i=0..n (x_m-1_i), aka the sum of the last elements on each layer
    // for the first (0-th) element:
    // x_0_0 = x_1_0 - x_0_1 = x_1_0 - (x_1_1 - x_0_2) = x_1_0 - (x_2_0 - (x_3_0 - (... - x_n_0)))
    // We get this by the alternating sum or simply by folding with - from the right which does exactly the formula above
    // Note: we can also get the correct back prediction by r-folding, (or by normal fold, which is equivalent to sum),
    // but for + it doesn't matter, because addition is associative
    (last_elems.iter().sum(), first_elems.iter().rfold(0, |acc, &x| x - acc))
}

fn solve(filename: &str) -> (i64, i64){
    let mut res = (0i64, 0i64);
    if let Ok(lines) = utils::file_read::read_file_as_lines(filename){
        for line in lines {
            let res_reading = get_extrapolation(&utils::string_utils::split_string_i64(&line));
            res.0 += res_reading.0;
            res.1 += res_reading.1;
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