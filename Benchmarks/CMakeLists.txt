cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

include_directories(../Source)

set(CommonHeaders Helpers/Measure.h)
set(CommonSource Helpers/Measure.cpp)

project(MVLTBench)

add_executable(MVaultBench MVault/main.cpp ${CommonHeaders} ${CommonSource})
target_link_libraries(MVaultBench MVault)


project(REDISBench)

find_library(HIREDIS_LIBRARY NAMES hiredis)

add_executable(RedisBench Redis/main.cpp Redis/RedisDb.cpp Redis/RedisDb.h ${CommonHeaders} ${CommonSource})
target_link_libraries(RedisBench hiredis)
target_link_libraries(RedisBench MVault)