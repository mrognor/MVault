#pragma once

#include "TestAssert.h"

#define TEST_COUT(method, res)\
{ \
    std::ostringstream oss; \
    std::streambuf* p_cout_streambuf = std::cout.rdbuf(); \
    std::cout.rdbuf(oss.rdbuf()); \
    method; \
    std::cout.rdbuf(p_cout_streambuf); \
    TEST_ASSERT(res == oss.str()) \
}

#define SUPPRESS_COUT(method)\
{ \
    std::ostringstream oss; \
    std::streambuf* p_cout_streambuf = std::cout.rdbuf(); \
    std::cout.rdbuf(oss.rdbuf()); \
    method; \
    std::cout.rdbuf(p_cout_streambuf); \
}
