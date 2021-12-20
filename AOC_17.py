import sys
import re


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        input_data = f.read().strip()
        xvals = tuple(int(x) for x in re.search('((x=)+)([0-9.-]+)', input_data).group().split('=')[1].split('..'))
        yvals = tuple(int(x) for x in re.search('((y=)+)([0-9.-]+)', input_data).group().split('=')[1].split('..'))
        tgt_area = (xvals, yvals)

        # Idea pt 1: we want to maximize y_vel (x actually doesn't matter for y_max, since x and y are independent)
        # We "just" have to make sure that y lands in the target area
        # With step_number = t: Calculating y(t) yields: y(t) = t/2 * (2y_vel -t + 1)
        # Since yvals[n] < 0, we search the largest y_values that is >= yvals[0]. We can calculate that for this,
        # t must be > 2 y_vel + 1, meaning that t = 2 y_vel + 2
        # Inserting this into y(t) yields: y_max_<0 = -yvel - 1 which has to be = y_min

        y_vel_opt = -yvals[0] - 1

        # the maximum is at t = y_vel +1/2 , will always yield 2 pts due to int => just take one of them
        # ymax = (yvel^2 + yvel) / 2, which is just gauss sum formula, aka triangular numbers

        y_max = 0.5 * (pow(y_vel_opt, 2) + y_vel_opt)
        print("Solution pt 1: " + str(y_max))

