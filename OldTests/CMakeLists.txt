cmake_minimum_required(VERSION 3.6)

if(WIN32)
    set(CMAKE_CXX_FLAGS "-Wall -g")
else()
    set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address  -fsanitize=leak -g")
endif()

set(CMAKE_CXX_STANDARD 11)

set(TestsHeaders 
    ColorizedPrint.h 
    Tests.h
    TestAssert.h)

set(TestsSource
    ColorizedPrint.cpp 
    MultithreadingTest.cpp 
    Bench.cpp)

project(Tests)

include_directories(../Source)	

add_executable(Tests ${TestsHeaders} ${TestsSource} Tests.cpp)
target_link_libraries(Tests MVault)

add_executable(VaultRecordRefTest VaultRecordRefTest.cpp)
target_link_libraries(VaultRecordRefTest MVault)

add_executable(SingleThreadedTests ${TestsHeaders} ${TestsSource} SingleThreadedTests.cpp)
target_link_libraries(SingleThreadedTests MVault)