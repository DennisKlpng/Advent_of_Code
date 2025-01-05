use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

pub fn read_file_as_lines<T>(filename: T) -> io::Result<Vec<String>>
where T: AsRef<Path>, {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines().flatten().collect::<Vec<String>>())
}

pub fn read_file_block_lines<T>(filename: T) -> io::Result<Vec<Vec<String>>>
where T: AsRef<Path>, {
    let mut res: Vec<Vec<String>> = vec![];
    let mut block_lines : Vec<String> = vec![];
    if let Ok(lines) = read_file_as_lines(filename){
        for line in lines{
            if line.is_empty(){
                res.push(block_lines.clone());
                block_lines.clear();
            }
            else{
                block_lines.push(line);
            }
        }
        //eof
        res.push(block_lines.clone());
        Ok(res)
    }
    else{
        panic!("Unable to parse input into blocks")
    }
}