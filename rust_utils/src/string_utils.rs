pub fn split_string_i64(parse_str: &str) -> Vec<i64>{
    return parse_str.split_whitespace().map(|val| val.parse().unwrap()).collect::<Vec<i64>>();
}

pub fn split_string_u64(parse_str: &str) -> Vec<u64>{
    return parse_str.split_whitespace().map(|val| val.parse().unwrap()).collect::<Vec<u64>>();
}

pub fn split_string_f64(parse_str: &str) -> Vec<f64>{
    return parse_str.split_whitespace().map(|val| val.parse().unwrap()).collect::<Vec<f64>>();
}