import sys
import re
from collections import defaultdict


# GENERAL IDEA:

def get_cube_volume(cube):  # set (xmin, xmax, ymin, ymax, zmin, zmax)
    vol = (cube[1]-cube[0]+1) * (cube[3]-cube[2]+1) * (cube[5]-cube[4]+1)
    return (cube[1]-cube[0]+1) * (cube[3]-cube[2]+1) * (cube[5]-cube[4]+1)


def intersect_range(c_min, c_max, ref_min, ref_max):
    res = set()
    if c_min < ref_min:  # pt on the left exists
        if c_max <= ref_max:  # no pt on the right
            res.add((c_min, ref_min-1))
            remainder = ref_min, c_max
        else:  # ref fully contained in c
            res.add((c_min, ref_min-1))
            res.add((ref_max+1, c_max))
            remainder = ref_min, ref_max
    else:
        if c_max > ref_max:  # c is partially right of ref, in case of fully contained don't append
            res.add((ref_max+1, c_max))
            remainder = c_min, ref_max
        else:  # c is fully contained in ref, no res, remainder = c
            remainder = c_min, c_max
    return res, remainder


def intersect_cube(cube, cube_ref):  # returns the volume of cube that is NOT in cube_ref as list of sliced cubes
    remainder_cubes = set()
    # trivial case: completely separate. In each dim: ref left when max_ref < min, right when min_ref > max
    if (cube_ref[1] < cube[0] or cube_ref[0] > cube[1]) or (cube_ref[3] < cube[2] or cube_ref[2] > cube[3]) \
            or (cube_ref[5] < cube[4] or cube_ref[4] > cube[5]):
        remainder_cubes.add(cube)
        return remainder_cubes

    # cut in each direction:
    x_cut = intersect_range(cube[0], cube[1], cube_ref[0], cube_ref[1])
    y_cut = intersect_range(cube[2], cube[3], cube_ref[2], cube_ref[3])
    z_cut = intersect_range(cube[4], cube[5], cube_ref[4], cube_ref[5])

    # combine into resulting cuboids, at most 6 (if tgt is completely enclosed)
    tgt_cube = list(cube)
    for cut in x_cut[0]:
        tgt_cube[0] = cut[0]
        tgt_cube[1] = cut[1]
        remainder_cubes.add(tuple(tgt_cube))
    tgt_cube[0] = x_cut[1][0]
    tgt_cube[1] = x_cut[1][1]
    for cut in y_cut[0]:
        tgt_cube[2] = cut[0]
        tgt_cube[3] = cut[1]
        remainder_cubes.add(tuple(tgt_cube))
    tgt_cube[2] = y_cut[1][0]
    tgt_cube[3] = y_cut[1][1]
    for cut in z_cut[0]:
        tgt_cube[4] = cut[0]
        tgt_cube[5] = cut[1]
        remainder_cubes.add(tuple(tgt_cube))

    return set(remainder_cubes)  # list of entries with the same structure


def intersect_cubelist(list_cubes, cube_to_check_against):  # both: set (xmin, xmax, ymin, ymax, zmin, zmax), 1 is list
    remainder_cubes = set()
    for cube in list_cubes:
        res_single_step = intersect_cube(cube, cube_to_check_against)
        for res_cube in res_single_step:
            remainder_cubes.add(res_cube)

    return remainder_cubes  # list of entries with the same structure


def calc_cores(reboot_steps):
    def_activated = set()
    def_deactivated = set()
    # parsing backwards removes the need to pop elements
    for step in range(len(reboot_steps) - 1, -1, -1):
        curr_cube_with_state = reboot_steps[step]
        curr_cube = [curr_cube_with_state[1]]
        # compare with all activated and deactivated entries, keep just the remainder cubes and add them to
        # according list (e.g. only the "not definitely active or not active" part
        for act in def_activated:
            curr_cube = intersect_cubelist(curr_cube, act)

        for unact in def_deactivated:
            curr_cube = intersect_cubelist(curr_cube, unact)

        if curr_cube_with_state[0] == 1:
            for cube in curr_cube:
                def_activated.add(cube)
        else:
            for cube in curr_cube:
                def_deactivated.add(cube)

    num_cubes = 0
    for cube in def_activated:
        num_cubes += get_cube_volume(cube)

    return num_cubes


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        reboot_steps = []
        reboot_steps_small = []
        for line in f.readlines():
            split = re.split(' |,', line.strip())
            switch = 1 if split[0] == 'on' else 0
            x_range = [int(x.strip()) for x in split[1].split('=')[1].split('..')]
            y_range = [int(x.strip()) for x in split[2].split('=')[1].split('..')]
            z_range = [int(x.strip()) for x in split[3].split('=')[1].split('..')]
            set_step = (switch, (x_range[0], x_range[1], y_range[0], y_range[1], z_range[0], z_range[1]))
            if min(min(x_range), min(y_range), min(z_range)) >= -50 \
                    and max(max(x_range), max(y_range), max(z_range)) <= 50:
                reboot_steps_small.append(set_step)
            reboot_steps.append(set_step)

        # pt 1
        num_pt1 = calc_cores(reboot_steps_small)
        print("Solution pt1: " + str(num_pt1))

        # pt 2
        num_pt2 = calc_cores(reboot_steps)
        print("Solution pt2: " + str(num_pt2))

