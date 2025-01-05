pub fn split_string_i64(parse_str: &str) -> Vec<i64>{
    let mut res: Vec<i64> = vec![];
    for val in parse_str.split_whitespace(){
        if let Ok(x) = val.parse::<i64>(){
            res.push(x);
        }
    }
    res
}

pub fn split_string_u64(parse_str: &str) -> Vec<u64>{
    let mut res: Vec<u64> = vec![];
    for val in parse_str.split_whitespace(){
        if let Ok(x) = val.parse::<u64>(){
            res.push(x);
        }
    }
    res
}

pub fn split_string_f64(parse_str: &str) -> Vec<f64>{
    let mut res: Vec<f64> = vec![];
    for val in parse_str.split_whitespace(){
        if let Ok(x) = val.parse::<f64>(){
            res.push(x);
        }
    }
    res
}