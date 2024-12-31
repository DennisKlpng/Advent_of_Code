pub fn get_diag_neighbours(pt: (usize, usize)) -> Vec<(usize, usize)>{
    
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

pub fn get_neighbours(pt: (usize, usize)) -> Vec<(usize, usize)>{
    
    let res: Vec<(usize, usize)> = vec![(pt.0 - 1usize, pt.1),
                                        (pt.0, pt.1 - 1usize),
                                        (pt.0, pt.1 + 1usize),
                                        (pt.0 + 1usize, pt.1)];
    res
}