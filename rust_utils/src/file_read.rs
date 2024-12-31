use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

pub fn read_file_as_lines<T>(filename: T) -> io::Result<Vec<String>>
where T: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines().flatten().collect::<Vec<String>>())
}