import sys
from itertools import permutations
from _collections import defaultdict


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
            break
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

        def check_alignment(scan_aligned, scan_unaligned):
            num_compatible_beacons = 0
            # check if they are aligned by a transformation. If yes, return aligned scan
            # idea: assume that an arbitrary point from scan_unaligned is compatible with a pt from scan_aligned
            # calculate delta, if there are 12 deltas like that the scans match
            deltas = defaultdict(int)
            for beacon_unal in scan_unaligned:
                for beacon_al in scan_aligned:
                    deltas[tuple(tuple(beacon_al)[i] - tuple(beacon_unal)[i] for i in range(3))] += 1
            for delta, occurence in deltas.items():
                if occurence >= 12:
                    scanner_pos.append(delta)
                    return [(tuple(unal_beacon[i] + delta[i] for i in range(3))) for unal_beacon in scan_unaligned]

            return None

        # calc transformed scans beforehand to save calculation time
        converted_scans = {}
        for index_scan in range(len(scanner_beacons_unaligned)):
            # calc iteration of scan first
            for orientation in range(48):
                converted_scans[(index_scan, orientation)] = [transform_point(point, orientation)
                                                              for point in scanner_beacons_unaligned[index_scan]]
        indices_unaligned = [x for x in range(len(scanner_beacons_unaligned))]
        scanner_pos = [(0, 0, 0)]

        # as long as the list of unaligned indices is not empty, compare them with each entry of aligned data
        # comparison: for each possible orientation of the unaligned entry, check for match
        while indices_unaligned:
            print("Num of unaligned: " + str(len(indices_unaligned)))
            scan_to_remove = -1
            break_loops = False
            scanner_beacons_aligned_next_step = scanner_beacons_aligned.copy()
            for index_scan in indices_unaligned:
                # iterate over variants of scan
                for orientation in range(48):
                    scan_converted = converted_scans[(index_scan, orientation)]
                    # compare against each scan in aligned
                    for aligned_scan in scanner_beacons_aligned:
                        newly_aligned_scan = check_alignment(aligned_scan, scan_converted)
                        if newly_aligned_scan:
                            # save index for removal
                            scan_to_remove = index_scan
                            # add beacons from newly aligned scan to set of beacons (if they are not present yet)
                            for entry in newly_aligned_scan:
                                beacon_coord_aligned.add(entry)
                            scanner_beacons_aligned_next_step.append(newly_aligned_scan)
                            break_loops = True
                            break
                    if break_loops:
                        break
                if break_loops:
                    break

            # remove newly aligned scans from unaligned
            if scan_to_remove > -1:
                indices_unaligned.remove(scan_to_remove)
            scanner_beacons_aligned = scanner_beacons_aligned_next_step

        print("Number of individual beacons: " + str(len(beacon_coord_aligned)))

        max_dist = 0
        for scanner1 in scanner_pos:
            for scanner2 in scanner_pos:
                dist = abs(scanner2[0] - scanner1[0]) + abs(scanner2[1] - scanner1[1]) + abs(scanner2[2] - scanner1[2])
                if dist > max_dist:
                    max_dist = dist

        print("Max distance: " + str(max_dist))
