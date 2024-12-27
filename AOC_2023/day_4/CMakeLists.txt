cmake_minimum_required (VERSION 3.31)
project (CMakeDay2)

# file(STRINGS ../inputs/Test_4.txt test_in)
file(STRINGS ../inputs/Data_4.txt test_in)

function(solve input_data part_2)
    SET(index 0)
    SET(RES_2 "0")
    list(LENGTH input_data NUM_ORIG_CARDS)
    SET(list_card_counts "")
    while(${index} LESS ${NUM_ORIG_CARDS})
        list(LENGTH list_card_counts card_count_size)
        if(${card_count_size} LESS_EQUAL ${index}) # => index is not a valid index in the list
            list(APPEND list_card_counts "0")
        endif()
        # Increment the number at index by 1 (which is incredibly inconvenient in CMake...)
        list(GET list_card_counts ${index} number_for_curr_index)
        math(EXPR number_for_curr_index "${number_for_curr_index} + 1")
        list(TRANSFORM list_card_counts REPLACE "[0-9]+" "${number_for_curr_index}" AT ${index})
        # number_for_curr_index is the number of cards for index

        # Parse actual data and do matching / checking against winning numbers
        list(GET input_data ${index} LINE)
        string(REPLACE "|" ";" LINES ${LINE})
        list(LENGTH LINES LIST_SIZE)
        list(GET LINES 0 NUMS_CARD)
        list(GET LINES 1 NUMS_WINS)
        string(REGEX MATCHALL "[0-9]+" MATCHES_NUMS ${NUMS_CARD})
        # Remove first match, since this is the card number
        list(REMOVE_AT MATCHES_NUMS 0)
        string(REGEX MATCHALL "[0-9]+" MATCHES_WINS ${NUMS_WINS})
        SET(CARD_SCORE "0")
        set(index_offset ${index})
        foreach(CARD_NUM ${MATCHES_NUMS})
            foreach(WIN_NUM ${MATCHES_WINS})
                if(WIN_NUM STREQUAL CARD_NUM)
                    # We need to add n cards with the following numbers into our list of cards
                    # index_offset is the new current card index
                    math(EXPR index_offset "${index_offset} + 1")
                    if(${card_count_size} LESS_EQUAL ${index_offset}) # => index is not a valid index in the list
                        list(APPEND list_card_counts "0")
                    endif()
                    # Get current number for new card index, add 1 for each number of our current card index
                    list(GET list_card_counts ${index_offset} number_for_curr_indexoffset)
                    math(EXPR number_for_curr_indexoffset "${number_for_curr_indexoffset} + ${number_for_curr_index}")
                    list(TRANSFORM list_card_counts REPLACE "[0-9]+" "${number_for_curr_indexoffset}" AT ${index_offset})

                    if(CARD_SCORE STREQUAL "0")
                        math(EXPR CARD_SCORE "${CARD_SCORE} + 1")
                    else()
                        math(EXPR CARD_SCORE "${CARD_SCORE} * 2")
                    endif()
                endif()
            endforeach()
        endforeach()
        math(EXPR RES "${RES} + ${CARD_SCORE}")
        math(EXPR index "${index} + 1")
    endwhile() # LINE aka Card
    #Sum number of card counts
    foreach(card ${list_card_counts})
        math(EXPR RES_2 "${RES_2} + ${card}")
    endforeach()
    if(part_2)
        message("Res part 2: " ${RES_2})
    else()
        message("Res part 1: " ${RES})
    endif()
endfunction()

solve("${test_in}" "FALSE")
solve("${test_in}" "TRUE")