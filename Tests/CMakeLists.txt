cmake_minimum_required(VERSION 2.6)

if(WIN32)
    set(CMAKE_CXX_FLAGS "-Wall -g")
else()
    set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address  -fsanitize=leak -g")
endif()

set(CMAKE_CXX_STANDARD 11)

set(TestsHeaders 
    ColorizedPrint.h 
    Tests.h)

set(TestsSource
    ColorizedPrint.cpp 
    MultithreadingTest.cpp 
    Tests.cpp
    Bench.cpp)

project(Tests)

include_directories(../Source)	

add_executable(Tests ${TestsHeaders} ${TestsSource})
target_link_libraries(Tests MVault)