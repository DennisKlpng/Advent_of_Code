import sys
from itertools import permutations


def transform_point(point, rot):
    ret = [point[0], point[1], point[2]]
    # x, y, z could mean +/- x, +/- y, +/- z (8 options) and all permutations of it (3! = 6 options)
    # encode target transform in rot => rot € [0, 47], encode transform bitwise, rot = pqrzyx
    # xyz are bites for x/y/z shifting, pqr are used for the permutations
    # possible combinations for pqr range: 101 (40+), 100 (32-39), 011 (24-31), 010 (16-23), 001 (8-15), 000 (0-7)
    # bit shifting by 3 will give according range
    for index, perm in enumerate(list(permutations([0, 1, 2]))):
        if index == rot >> 3:
            ret = [ret[perm[0]], ret[perm[1]], ret[perm[2]]]
        # access n-th last bit by by (a >> n) & 1
        if 1 == (rot >> 2) & 1:
            ret[2] *= -1
        if 1 == (rot >> 1) & 1:
            ret[1] *= -1
        if 1 == rot & 1:
            ret[0] *= -1
    return tuple(ret)


if __name__ == '__main__':
    with open(sys.argv[1], "r") as f:
        scanner_beacons_unaligned = []
        blocks = f.read().split("\n\n")
        for block in blocks:
            lines = block.split("\n")
            scanner_data =[]
            for line in range(len(lines) - 1):
                beacon_coords = tuple(int(x) for x in lines[line+1].split(","))
                scanner_data.append(beacon_coords)
            scanner_beacons_unaligned.append(scanner_data)

        # scanner 0 is by definition aligned
        scanner_beacons_aligned = [scanner_beacons_unaligned.pop(0)]
        # add all beacons from scanner 0
        beacon_coord_aligned = {x for x in scanner_beacons_aligned[0]}
        print(beacon_coord_aligned)

        def check_alignment(scan_aligned, scan_unaligned):
            num_compatible_beacons = 0
            # check if they are aligned. If yes, return True
            

            return False

        # as long as the list of unaligned scanner_data is not empty, compare them with each entry of aligned data
        # comparison: for each possible orientation of the unaligned entry, check for match
        while scanner_beacons_unaligned:
            list_scan_aligned_in_step = []
            for index_scan in len(scanner_beacons_unaligned):
                # calc iteration of scan first
                for orientation in range(48):
                    scan_converted = [transform_point(point, orientation)
                                      for point in scanner_beacons_unaligned[index_scan]]
                    # compare against each scan in aligned
                    for aligned_scan in scanner_beacons_aligned:
                        if check_alignment(aligned_scan, scan_converted):
                            # save index for removal
                            list_scan_aligned_in_step.append(index_scan)
                            # add beacons from newly aligned scan to set of beacons (if they are not present yet)
                            for entry in scan_converted:
                                beacon_coord_aligned.add(entry)
            # remove all newly aligned scans
            for removal_index in list_scan_aligned_in_step:
                scanner_beacons_aligned.append(scanner_beacons_unaligned.pop(removal_index))

        print("Number of individual beacons: " + str(len(beacon_coord_aligned)))




        # for each scanner, compare

