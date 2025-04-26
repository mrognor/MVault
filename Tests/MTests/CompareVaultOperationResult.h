#pragma once

#define COMPARE_OPERATION(vor, equation1, equation2, equation3, equation4, equation5) \
    TEST_ASSERT(vor.equation1 && vor.equation2 && vor.equation3 && vor.equation4 && vor.equation5)
