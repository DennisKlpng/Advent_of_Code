use std::{cmp::Ordering, collections::HashMap};

use rust_utils as utils;

static CARD_VALS : [char; 13] = ['2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'];

#[derive(Eq, PartialEq)]
struct Hand{
    cards: String,
    hand_type: u8,
    bid: u32
}

impl Hand{
    fn calc_hand_type(&mut self){
        //count number of occurences
        let counts = self.cards.chars().fold(HashMap::new(), |mut acc, card| {
            *acc.entry(card).or_insert(0) += 1;
            acc
        });
        let mut counts = counts.values().into_iter().collect::<Vec<_>>();
        counts.sort_unstable();
        self.hand_type = match counts.as_slice(){
            [5] => 7,
            [1, 4] => 6,
            [2, 3] => 5,
            [1, 1, 3] => 4,
            [1, 2, 2] => 3,
            [1, 1, 1, 2] => 2,
            [1, 1, 1, 1, 1] => 1,
            _ => 0
        }
    }

    fn get_card_value(x: &char) -> u8{
        CARD_VALS.iter().position(|y| y == x).unwrap() as u8
    }
}

impl Ord for Hand{
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        if self.hand_type < other.hand_type{
            return Ordering::Less;
        }
        else if self.hand_type > other.hand_type{
            return Ordering::Greater;
        }
        else{
            for (self_val, other_val) in self.cards.chars().zip(other.cards.chars()){
                let self_card_val = Hand::get_card_value(&self_val);
                let other_card_val = Hand::get_card_value(&other_val);
                if self_card_val < other_card_val{
                    return Ordering::Less;
                }
                else if self_card_val > other_card_val{
                    return Ordering::Greater;
                }
            }
        }
        return Ordering::Equal;
    }
}
impl PartialOrd for Hand{
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

fn solve(filename: &str) -> (u64, u64){
    let mut res: (u64, u64) = (0u64, 0u64);
    if let Ok(lines) = utils::file_read::read_file_as_lines(filename){
        let mut hands = vec![];
        for line in lines {
            //lexographical sort cards => we can just use the default Eq and PartialEq implementations
            let mut hand = Hand{cards: line[0..5].to_owned(), hand_type: 0, bid: line[6..].parse::<u32>().unwrap()};
            hand.calc_hand_type();
            hands.push(hand);
        }
        hands.sort();
        res.0 = hands.iter().zip(1u32..).fold(res.0, |acc, (hand, rank)| acc + (hand.bid*rank) as u64);
    };

    res
}

fn main(){
    let res = solve("../inputs/Test_7.txt");
    println!("Test res pt 1: {:?} pt2: {:?}", res.0, res.1);
    use std::time::Instant;
    let now = Instant::now();
    let res = solve("../inputs/Data_7.txt");
    let elapsed = now.elapsed();
    println!("Puzzle res: pt 1: {:?} pt2: {:?} execution time: {:.2?}", res.0, res.1, elapsed);
}