#pragma once

#include <fstream>

#define COMPARE_FILE(fileName, isOpen, data) \
{ \
    std::ifstream f(fileName, std::ios::binary | std::ios::ate); \
    TEST_ASSERT(f.is_open() == isOpen); \
    if (f.is_open()) \
    { \
        std::size_t fileLen = f.tellg(); \
        f.seekg(0); \
        std::string fileData(fileLen, 0); \
        f.read(&fileData[0], fileLen); \
        TEST_ASSERT(data == fileData); \
    } \
}

#define SAVE_FILE(...) \
std::ofstream oFile(fileName); \
oFile << __VA_ARGS__; \
std::flush(oFile);
