#pragma once

#define COMPARE_OPERATION1(vor, equation) TEST_ASSERT(vor.equation)
#define COMPARE_OPERATION2(vor, equation1, equation2) TEST_ASSERT(vor.equation1 && vor.equation2)
#define COMPARE_OPERATION3(vor, equation1, equation2, equation3) \
    TEST_ASSERT(vor.equation1 && vor.equation2 && vor.equation3)
#define COMPARE_OPERATION4(vor, equation1, equation2, equation3, equation4) \
    TEST_ASSERT(vor.equation1 && vor.equation2 && vor.equation3 && vor.equation4)
#define COMPARE_OPERATION5(vor, equation1, equation2, equation3, equation4, equation5) \
    TEST_ASSERT(vor.equation1 && vor.equation2 && vor.equation3 && vor.equation4 && vor.equation5)

#define COMPARE_SELECT(vor, _1, _2, _3, _4, _5, macro, ...) macro
#define COMPARE_OPERATION(vor, ...) \
    COMPARE_SELECT(vor, __VA_ARGS__, COMPARE_OPERATION5, COMPARE_OPERATION4, COMPARE_OPERATION3, COMPARE_OPERATION2, COMPARE_OPERATION1, dummy) \
    (vor, __VA_ARGS__)
