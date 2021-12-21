import sys
from collections import defaultdict


class Player:
    def __init__(self, init_pos):
        self.score = 0
        self.pos = init_pos

    def roll_det_dice(self, former_throw):
        res = (3 * former_throw + 6 - 1) % 100 + 1
        self.pos = 1 + (self.pos + res - 1) % 10
        self.score += self.pos
        return (former_throw + 2) % 100 + 1


class DiracPlayers:
    # pt 2 quantum dice: for each "throw" there are 27 universes (3^3). The distribution of the sum is:
    # 1 / 27 = 3; 3/27 = 4; 6/27 = 5; 7/27 = 6; 6/27 = 7; 3/27 = 8; 1/27 = 9
    distribution = [(3, 1), (4, 3), (5, 6), (6, 7), (7, 6), (8, 3), (9, 1)]

    def __init__(self, init_pos1, init_pos2):
        self.state = defaultdict(int)  # key: (pos_board1, pos_board2, score1, score2)
        self.state[(init_pos1, init_pos2, 0, 0)] = 1

    def calc(self):
        wins1, wins2 = 0, 0
        # new state: increase all existing states according to the distribution (with regards to modulo)
        # state with pos n and count m leads to e.g. 3m states with pos n + 4
        while len(self.state) > 0:
            # player 1:
            updated_state = defaultdict(int)
            for indiv_state, count in self.state.items():
                for item in self.distribution:
                    new_pos = (indiv_state[0] + item[0] - 1) % 10 + 1
                    score = indiv_state[2] + new_pos
                    if score >= 21:
                        wins1 += count*item[1]  # do not use that state further => num combinations * initial count
                    else:
                        updated_state[new_pos, indiv_state[1], score, indiv_state[3]] += count*item[1]
            self.state = updated_state
            print(len(self.state))

            # player 2:
            updated_state = defaultdict(int)
            for indiv_state, count in self.state.items():
                for item in self.distribution:
                    new_pos = (indiv_state[1] + item[0] - 1) % 10 + 1
                    score = indiv_state[3] + new_pos
                    if score >= 21:
                        wins2 += count*item[1]  # do not use that state further
                    else:
                        updated_state[indiv_state[0], new_pos, indiv_state[2], score] += count * item[1]
            self.state = updated_state
            print(len(self.state))
        return wins1, wins2


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        start_pos = []
        for line in f.readlines():
            start_pos.append(int(line.split(': ')[1].strip()))
        player_1 = Player(start_pos[0])
        player_2 = Player(start_pos[1])
        dice_rolls = 0
        throw_det = 0
        # det classical dice, pt 1
        while True:
            throw_det = player_1.roll_det_dice(throw_det)
            dice_rolls += 3
            if player_1.score >= 1000:
                print("Player wins, res pt 1: " + str(dice_rolls * player_2.score))
                break
            throw_det = player_2.roll_det_dice(throw_det)
            dice_rolls += 3
            if player_2.score >= 1000:
                print("Player wins, res pt 1: " + str(dice_rolls * player_1.score))
                break

        # pt 2
        play3 = DiracPlayers(start_pos[0], start_pos[1])
        ret = play3.calc()

        print("Wins player 1: " + str(ret[0]) + " Wins player 2: " + str(ret[1]))
