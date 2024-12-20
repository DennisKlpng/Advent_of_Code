import sys


def gauss_sum(in_data):
    return int((in_data * in_data + in_data)/2)


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        crabs = [int(i) for i in f.readline().split(",")]

        min_fuel_simple = 0
        min_fuel_complex = 0
        opt_pos_simple = -1
        opt_pos_complex = -1
        for tgt_pos in range(max(crabs)):
            alignment_fuel_simple = sum(abs(pos - tgt_pos) for pos in crabs)
            if alignment_fuel_simple < min_fuel_simple or min_fuel_simple == 0:
                min_fuel_simple = alignment_fuel_simple
                opt_pos_simple = tgt_pos
            alignment_fuel_complex = sum(gauss_sum(abs(pos - tgt_pos)) for pos in crabs)
            if alignment_fuel_complex < min_fuel_complex or min_fuel_complex == 0:
                min_fuel_complex = alignment_fuel_complex
                opt_pos_complex = tgt_pos

        print("Optimal pos pt 1: " + str(opt_pos_simple) + " Fuel consumption: " + str(min_fuel_simple))
        print("Optimal pos pt 2: " + str(opt_pos_complex) + " Fuel consumption: " + str(min_fuel_complex))
