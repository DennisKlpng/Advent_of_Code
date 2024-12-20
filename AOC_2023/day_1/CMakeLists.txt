cmake_minimum_required (VERSION 3.31)
project (CMakeDay1)

# file(STRINGS ../inputs/Test_1.txt test_1_in)
file(STRINGS ../inputs/Data_1.txt test_1_in)
# file(STRINGS ../inputs/Test_1_b.txt test_1_in)

function(regex_replace input_match)
    set(NUM_MATCH ${input_match})
    string(REPLACE "one" "1" NUM_MATCH ${NUM_MATCH})
    string(REPLACE "two" "2" NUM_MATCH ${NUM_MATCH})
    string(REPLACE "three" "3" NUM_MATCH ${NUM_MATCH})
    string(REPLACE "four" "4" NUM_MATCH ${NUM_MATCH})
    string(REPLACE "five" "5" NUM_MATCH ${NUM_MATCH})
    string(REPLACE "six" "6" NUM_MATCH ${NUM_MATCH})
    string(REPLACE "seven" "7" NUM_MATCH ${NUM_MATCH})
    string(REPLACE "eight" "8" NUM_MATCH ${NUM_MATCH})
    string(REPLACE "nine" "9" NUM_MATCH ${NUM_MATCH})
    set(NUM_MATCH ${NUM_MATCH} PARENT_SCOPE)
endfunction()

function(solve input_data part_2)
    set(RES "0")
    foreach(LINE ${input_data})
        if(part_2)
            # Idea: replace at beginning + end separately, then join them together
            # E.g. onexfour => 1xfour + onex4 = 1xfouronex4
            # The problem here regexing that: regex doesn't match overlaps by default. 
            # Can be done by e.g. lookahead capture groups, but they don't exist in CMake regex
            # We have overlaps with several words ending in e and eight AND seven + nine AND eight + two AND two + one
            # Solution: replace any "nine" with "nnine" and "eight" with "eeight" and "two" with "ttwoo"
            string(REPLACE "nine" "nnine" LINE ${LINE})
            string(REPLACE "eight" "eeight" LINE ${LINE})
            string(REPLACE "two" "ttwoo" LINE ${LINE})
            set(LINE_START ${LINE})
            set(LINE_END ${LINE})
            # match all occurences of numbers (now gets all, no overlaps anymore) => return value is a list of matches
            string(REGEX MATCHALL "one|two|three|four|five|six|seven|eight|nine" MATCHES ${LINE_START})
            list(LENGTH MATCHES LIST_SIZE)
            if(NOT "${LIST_SIZE}" STREQUAL "0")
                list(GET MATCHES 0 FIRST_MATCH)
                list(GET MATCHES -1 LAST_MATCH)

                # Replace the first occurence with a placeholder, convert it to a digit, replace back
                string(REPLACE "${FIRST_MATCH}" "*" LINE_START ${LINE_START})
                regex_replace("${FIRST_MATCH}")
                string(REPLACE "*" ${NUM_MATCH} LINE_START ${LINE_START})
                # Replace the last occurence with a placeholder, convert it to a digit, replace back
                string(REPLACE "${LAST_MATCH}" "*" LINE_END ${LINE_END})
                regex_replace("${LAST_MATCH}")
                string(REPLACE "*" ${NUM_MATCH} LINE_END ${LINE_END})
            endif()
            # The resulting string now has the first "word"-digit and it's last "word"-digit replaced 
            # and their positions are invariant relative to the first and last digit
            string(JOIN "" LINE ${LINE_START} ${LINE_END})
        endif()
        # Find all digits
        string(REGEX MATCHALL "[0-9]" MATCHES ${LINE})
        list(LENGTH MATCHES LIST_SIZE)
        if(NOT "${LIST_SIZE}" STREQUAL "0")
            list(GET MATCHES 0 FIRST)
            list(GET MATCHES -1 LAST)
            # Join the first and last digit together
            string(JOIN "" TMP_RES ${FIRST} ${LAST})
            # Add them to the RES variable (EXPR does math in a stringy-way)
            math(EXPR RES "${RES} + ${TMP_RES}")
        endif()
    endforeach()
    if(part_2)
        message("Res part 2: " ${RES})
    else()
        message("Res part 1: " ${RES})
    endif()
endfunction()

solve("${test_1_in}" "FALSE")
solve("${test_1_in}" "TRUE")