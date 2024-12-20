import sys
import re
import math


def calc_pos_vel(init_vel, t, is_x):
    if is_x:
        if t > init_vel:  # account for x_vel never being < 0
            return 0.5*(init_vel * init_vel + init_vel)
        return t * init_vel - 0.5*(t * t - t)
    else:
        return t * init_vel - 0.5*(t * t - t)


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

        # pt 2
        # for y: any solution with y_vel > -y_tgt_min - 1 is impossible
        # any solution with y_vel < -tgt_min is also impossible, since this will just directly overshoot
        y_max = -yvals[0] - 1
        y_min = yvals[0]

        # for x: we actually need to reach the tgt, which means, that x(t) >= x_tgt_min and no overshoot
        x_max = xvals[1]  # trivial
        # since x_vel_(t) = x_vel - t, we can just calc the x pos from gauss as before
        # (x_pos(t) = t * x_vel - sum(0..t)(t-1) = t*x_vel - (t^2 - t)/2 (as long as t > 0)
        # the maximum is the same as above, at x_pos_max = (x_vel^2 + x_vel)/2, inverting that yields:
        x_min = int(math.sqrt(2 * xvals[0] + 0.25) - 0.5) + 1

        # Now brute force the solution
        num_valid_solutions = 0
        for x in range(x_min, x_max+1):
            for y in range(y_min, y_max+1):
                step = 0
                while True:
                    step += 1
                    x_pos = calc_pos_vel(x, step, True)
                    y_pos = calc_pos_vel(y, step, False)
                    if xvals[0] <= x_pos <= xvals[1] and yvals[0] <= y_pos <= yvals[1]:
                        num_valid_solutions += 1
                        break
                    if x_pos > xvals[1] or y_pos < yvals[0]:
                        break  # will not hit

        print("Solution pt 2: " + str(num_valid_solutions))

