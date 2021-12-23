import sys
import copy


tgt_state = {2: ['A', 'A'], 4: ['B', 'B'], 6: ['C', 'C'], 8: ['D', 'D']}
room_assoc = {2: 'A', 4: 'B', 6: 'C', 8: 'D'}
costs = {'A': 1, 'B': 10, 'C': 100, 'D': 1000}
cost_min = sys.maxsize


def get_distance(curr_pos, tgt_pos, element):
    dist = abs(tgt_pos[0] - curr_pos[0]) + tgt_pos[1] + curr_pos[1]
    return dist * costs[element]


def get_potential_moving_elements(board):
    pot_elems = []
    for col in range(len(board)):
        for row in range(len(board[col])):
            if board[col][row] == '.':
                continue
            if row > 1 and board[col][row-1] != '.':
                continue
            if (col == 0 and board[col+1][0] != '.') or (col == 10 and board[col-1][0] != '.'):
                continue  # element can't move at all
            if row > 0:
                if board[col][row] == tgt_state[col][0]:
                    # check if is in tgt pos and has no non-tgt element below => do not move
                    invalid_val = False
                    for further_row in range(row, len(board[col])):
                        if board[col][further_row] != tgt_state[col][0]:
                            invalid_val = True
                            break
                    if not invalid_val:
                        continue  # is in tgt pos, do not move
            pot_elems.append((col, row))
    return pot_elems


def check_reachability(board, curr_pos, tgt_pos):
    # vertical blockage impossible, only check hoizontal
    reachable = True
    if curr_pos[0] < tgt_pos[0]:
        for i in range(curr_pos[0] + 1, tgt_pos[0], 1):
            if board[i][0] != '.':
                reachable = False
                break
    else:
        for i in range(tgt_pos[0] + 1, curr_pos[0], 1):
            if board[i][0] != '.':
                reachable = False
                break

    return reachable


def get_potential_tgt_pos(board, element, curr_pos):  # str, (col, row)
    # get all empty spaces:
    tgt_pos = []
    for col in range(len(board)):
        if curr_pos[0] == col:
            continue  # movements within column are pointless
        if col in room_assoc.keys() and element != room_assoc[col]:
            continue  # room is incompatible with element
        for row in range(len(board[col])):
            if row == 0 and col in room_assoc.keys():
                continue  # pos on top of room
            if board[col][row] != '.':
                continue  # space already occupied
            if row > 0:  # check other elements in room. If invalid val skip col, else continue if below is empty
                invalid_val = False
                other_elems = list(room_assoc.values())
                other_elems.remove(element)
                for further_row in range(1, len(board[col])):
                    if board[col][further_row] in other_elems:
                        invalid_val = True
                        break
                if invalid_val:
                    break  # skip col completely
                if row < len(board[col]) - 1:
                    if board[col][row+1] == '.':
                        continue
            if curr_pos[1] == 0 and curr_pos[1] == row:
                continue  # elements can't move to other pos in hallway
            pot_tgt_pos = (col, row)
            # check if tgt_pos is reachable from curr_pos
            if not check_reachability(board, curr_pos, pot_tgt_pos):
                continue
            tgt_pos.append((pot_tgt_pos, get_distance(curr_pos, pot_tgt_pos, element)))
    return tgt_pos


def search_path(board, cost, board_tgt):
    if board == board_tgt:
        global cost_min
        if cost < cost_min:
            cost_min = cost
            print("Reached end, cost: " + str(cost))
        return 0

    pot_movable_list = get_potential_moving_elements(board)
    for pot_movable in pot_movable_list:
        tgt_position_list = get_potential_tgt_pos(board, board[pot_movable[0]][pot_movable[1]], pot_movable)
        for pos in tgt_position_list:
            board_temp = copy.deepcopy(board)
            cost_temp = cost
            cost_temp += pos[1]
            if cost_temp > cost_min:
                continue
            board_temp[pos[0][0]][pos[0][1]] = board_temp[pot_movable[0]][pot_movable[1]]
            board_temp[pot_movable[0]][pot_movable[1]] = "."
            res = search_path(board_temp, cost_temp, board_tgt)
            if not res:
                continue
            cost += res[0]
    return None


def test_case(board, board_tgt):
    # step 1: B from (6, 1) to (3, 0)
    pot_movable_list = get_potential_moving_elements(board)
    to_move = (6, 1)
    assert to_move in pot_movable_list
    tgt_move = ((3, 0), 40)
    pot_tgt_list = get_potential_tgt_pos(board, board[to_move[0]][to_move[1]], to_move)
    assert tgt_move in pot_tgt_list
    board[tgt_move[0][0]][tgt_move[0][1]] = board[to_move[0]][to_move[1]]
    board[to_move[0]][to_move[1]] = "."
    assert board[3][0] == "B"
    assert board[6][1] == "."

    # step 2: C from (4, 1) to (6, 1)
    pot_movable_list = get_potential_moving_elements(board)
    to_move = (4, 1)
    assert to_move in pot_movable_list
    tgt_move = ((6, 1), 400)
    pot_tgt_list = get_potential_tgt_pos(board, board[to_move[0]][to_move[1]], to_move)
    assert tgt_move in pot_tgt_list
    board[tgt_move[0][0]][tgt_move[0][1]] = board[to_move[0]][to_move[1]]
    board[to_move[0]][to_move[1]] = "."
    assert board[6][1] == "C"
    assert board[4][1] == "."

    # step 3: C from (4, 1) to (6, 1)
    pot_movable_list = get_potential_moving_elements(board)
    to_move = (4, 2)
    assert to_move in pot_movable_list
    tgt_move = ((5, 0), 3000)
    pot_tgt_list = get_potential_tgt_pos(board, board[to_move[0]][to_move[1]], to_move)
    assert tgt_move in pot_tgt_list
    board[tgt_move[0][0]][tgt_move[0][1]] = board[to_move[0]][to_move[1]]
    board[to_move[0]][to_move[1]] = "."
    assert board[5][0] == "D"
    assert board[4][2] == "."

    # step 4: B from (3, 0) to (4, 2)
    pot_movable_list = get_potential_moving_elements(board)
    to_move = (3, 0)
    assert to_move in pot_movable_list
    tgt_move = ((4, 2), 30)
    pot_tgt_list = get_potential_tgt_pos(board, board[to_move[0]][to_move[1]], to_move)
    assert tgt_move in pot_tgt_list
    board[tgt_move[0][0]][tgt_move[0][1]] = board[to_move[0]][to_move[1]]
    board[to_move[0]][to_move[1]] = "."
    assert board[4][2] == "B"
    assert board[3][0] == "."

    # step 5: B from (2, 1) to (4, 1)
    pot_movable_list = get_potential_moving_elements(board)
    to_move = (2, 1)
    assert to_move in pot_movable_list
    tgt_move = ((4, 1), 40)
    pot_tgt_list = get_potential_tgt_pos(board, board[to_move[0]][to_move[1]], to_move)
    assert tgt_move in pot_tgt_list
    board[tgt_move[0][0]][tgt_move[0][1]] = board[to_move[0]][to_move[1]]
    board[to_move[0]][to_move[1]] = "."
    assert board[4][1] == "B"
    assert board[2][1] == "."

    # step 6: D from (8, 1) to (7, 0)
    pot_movable_list = get_potential_moving_elements(board)
    to_move = (8, 1)
    assert to_move in pot_movable_list
    tgt_move = ((7, 0), 2000)
    pot_tgt_list = get_potential_tgt_pos(board, board[to_move[0]][to_move[1]], to_move)
    assert tgt_move in pot_tgt_list
    board[tgt_move[0][0]][tgt_move[0][1]] = board[to_move[0]][to_move[1]]
    board[to_move[0]][to_move[1]] = "."
    assert board[7][0] == "D"
    assert board[8][1] == "."

    # step 7: A from (8, 2) to (9, 0)
    pot_movable_list = get_potential_moving_elements(board)
    to_move = (8, 2)
    assert to_move in pot_movable_list
    tgt_move = ((9, 0), 3)
    pot_tgt_list = get_potential_tgt_pos(board, board[to_move[0]][to_move[1]], to_move)
    assert tgt_move in pot_tgt_list
    board[tgt_move[0][0]][tgt_move[0][1]] = board[to_move[0]][to_move[1]]
    board[to_move[0]][to_move[1]] = "."
    assert board[9][0] == "A"
    assert board[8][2] == "."

    # step 8: D from (7, 0) to (8, 2)
    pot_movable_list = get_potential_moving_elements(board)
    to_move = (7, 0)
    assert to_move in pot_movable_list
    tgt_move = ((8, 2), 3000)
    pot_tgt_list = get_potential_tgt_pos(board, board[to_move[0]][to_move[1]], to_move)
    assert tgt_move in pot_tgt_list
    board[tgt_move[0][0]][tgt_move[0][1]] = board[to_move[0]][to_move[1]]
    board[to_move[0]][to_move[1]] = "."
    assert board[8][2] == "D"
    assert board[7][0] == "."

    # step 8: D from (5, 0) to (8, 1)
    pot_movable_list = get_potential_moving_elements(board)
    to_move = (5, 0)
    assert to_move in pot_movable_list
    tgt_move = ((8, 1), 4000)
    pot_tgt_list = get_potential_tgt_pos(board, board[to_move[0]][to_move[1]], to_move)
    assert tgt_move in pot_tgt_list
    board[tgt_move[0][0]][tgt_move[0][1]] = board[to_move[0]][to_move[1]]
    board[to_move[0]][to_move[1]] = "."
    assert board[8][1] == "D"
    assert board[5][0] == "."

    # step 9: A from (0, 9) to (2, 1)
    pot_movable_list = get_potential_moving_elements(board)
    to_move = (9, 0)
    assert to_move in pot_movable_list
    tgt_move = ((2, 1), 8)
    pot_tgt_list = get_potential_tgt_pos(board, board[to_move[0]][to_move[1]], to_move)
    assert tgt_move in pot_tgt_list
    board[tgt_move[0][0]][tgt_move[0][1]] = board[to_move[0]][to_move[1]]
    board[to_move[0]][to_move[1]] = "."
    assert board[2][1] == "A"
    assert board[9][0] == "."
    assert board == board_tgt


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        board = [['.'] for x in range(11)]
        board_tgt = [['.'] for x in range(11)]
        f.readline()
        f.readline()
        for line in f.readlines():
            line_vals = [x.strip() for x in line.strip().strip('#').split('#')]
            if len(line_vals) == 1:
                break
            for i in range(4):
                board[2 + 2 * i].append(line_vals[i])
                board_tgt[2 + 2 * i].append(room_assoc[2 + 2 * i])
        #print(board)
        #print(board_tgt)
        search_path(board, 0, board_tgt)
        print(cost_min)
        #test_case(board, board_tgt)



