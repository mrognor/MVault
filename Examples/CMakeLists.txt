cmake_minimum_required(VERSION 3.28)

if(WIN32)
    set(CMAKE_CXX_FLAGS "-Wall -g")
else()
    set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address  -fsanitize=leak -g")
endif()

set(CMAKE_CXX_STANDARD 11)

project(Examples)

include_directories(../Source)	

add_executable(Example1 Example1.cpp)
target_link_libraries(Example1 MVault)

add_executable(SortExample SortExample.cpp)
target_link_libraries(SortExample MVault)