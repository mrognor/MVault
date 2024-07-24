cmake_minimum_required(VERSION 3.6)

if(WIN32)
    set(CMAKE_CXX_FLAGS "-Wall -g")
else()
    set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address  -fsanitize=leak -g")
endif()

set(CMAKE_CXX_STANDARD 11)

set(TestsHeaders MTests/ColorizedPrint.h MTests/TestAssert.h)

set(TestsSource MTests/ColorizedPrint.cpp)

include_directories(../Source)

project(Scenarios)

add_executable(Scenarios ${TestsHeaders} ${TestsSource} Scenarios.cpp)
target_link_libraries(Scenarios MVault)

project(VaultUnitTest)

add_executable(VaultUnitTest ${TestsHeaders} ${TestsSource} VaultUnitTest.cpp)
target_link_libraries(VaultUnitTest MVault)

project(VaultRecordSetUnitTest)

add_executable(VaultRecordSetUnitTest ${TestsHeaders} ${TestsSource} VaultRecordSetUnitTest.cpp)
target_link_libraries(VaultRecordSetUnitTest MVault)