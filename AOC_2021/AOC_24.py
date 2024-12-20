import sys
import queue


# By manual decompiling the input, we notice that the structure for each digit in the input is with variables 1,2 and 3:
# inp w , mul x 0, add x z, mod x 26, div z var_1, add x var_2, eql x w, eql x 0, mul y 0, add y 25, mul y x, add y 1,
# mul z y, mul y 0, add y w, add y var_3, mul y x, add z y
# This can be reduced to z = z_0/var_1 * (25x + 1) + x ( w + var_3), with x = (z_0 % 26 + var_2) != w
#
# We notice that var_1 € [1, 26]. If var_1 == 1: var_2 >= 10; else var_2 < 0. If var_2 >= 10, (z_0 % 26 + var_2) will
# always be at least 10, and therefore != w (since w is a digit (1-9)
# => if var_1 == 1: z = 26 * z_0 + w + var_3
# => this pushes a variable in the range [2, 2z_0) into z
# Half the operations are with var_1 == 1 and will therefore increase the value of z
# To remove a value of z added to z, it is necessary to have a matching reduce that assures that x = 0 by ensuring
# z_0 % 26 + var_2 = w. This resembles pushing and popping according values in a LIFO queue
# example: pushing I[0] and I[1] and popping I[2] and I[3] (with v3_i = var_3[i]).
# When we calc z_2, we get z_2 = 26 (z_1) + I[2] + v3_2 = 26 (z_0 + I[1] + v3_1) + I[2] + v3_2
# When we divide this by 26 (when x = 0), we get exactly z_1 and so on. The last pushed value has to be removed by
# the first operation
# To ensure that, we calc the pop-I2 operation:
# z_2 % 26 + v2_3 = I[3] => z_1 + v3_2 + I[2]  + v2_3 = I[3]




if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        instruction_blocks = filter(None, f.read().split("inp w"))
        v2_push_array = []
        v3_push_array = []
        v2_pop_array = []
        v3_pop_array = []
        num_push_pop_assoc = {}  # store tuples for num pairs
        num_queue = queue.LifoQueue()  # used to build pairs
        #digit_push = []
        pos = 1
        for block in instruction_blocks:
            list_block = [x.strip() for x in filter(None, block.split('\n'))]
            if '1' == list_block[3].split('div z ')[1].strip():
                num_queue.put((pos, len(v2_push_array)))
                v2_push_array.append(int(list_block[4].split('add x ')[1].strip()))
                v3_push_array.append(int(list_block[14].split('add y ')[1].strip()))
            else:
                queue_entry = num_queue.get()
                num_push_pop_assoc[queue_entry[1]] = (queue_entry[0], pos, len(v2_pop_array))
                v2_pop_array.append(int(list_block[4].split('add x ')[1].strip()))
                v3_pop_array.append(int(list_block[14].split('add y ')[1].strip()))
            pos += 1

        res_max_val = {}
        res_val_max = ""
        res_min_val = {}
        res_val_min = ""
        for push_list_entry, assoc in num_push_pop_assoc.items():
            digit_push = assoc[0]
            digit_pull = assoc[1]
            for pot_digit in range(9, 0, -1):
                # v3_2 + I[2] + v2_3 = I[3]; I[3] is the tgt digit and I[2] is the pot_digit
                # v3_2 is from v3_push_array
                v3_2 = v3_push_array[push_list_entry]
                v2_3 = v2_pop_array[assoc[2]]
                tgt_digit = pot_digit + v3_2 + v2_3
                if 1 <= tgt_digit <= 9:
                    res_max_val[digit_push] = pot_digit
                    res_max_val[digit_pull] = tgt_digit
                    break
            for pot_digit in range(1, 10):
                v3_2 = v3_push_array[push_list_entry]
                v2_3 = v2_pop_array[assoc[2]]
                tgt_digit = pot_digit + v3_2 + v2_3
                if 1 <= tgt_digit <= 9:
                    res_min_val[digit_push] = pot_digit
                    res_min_val[digit_pull] = tgt_digit
                    break
        for i in range(1, 15):
            res_val_max += str(res_max_val[i])
            res_val_min += str(res_min_val[i])
        print("Pt1, max: " + res_val_max)
        print("Pt2, max: " + res_val_min)



