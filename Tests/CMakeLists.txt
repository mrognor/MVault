cmake_minimum_required(VERSION 3.6)

if(WIN32)
    set(CMAKE_CXX_FLAGS "-Wextra -g")
else()
    set(CMAKE_CXX_FLAGS "-Wextra -fsanitize=address -fsanitize=leak -lgcov --coverage -g")
endif()

set(CMAKE_CXX_STANDARD 11)

set(TestsHeaders MTests/ColorizedPrint.h MTests/TestAssert.h MTests/Starter.h MTests/GetLine.h MTests/TestLog.h)

set(TestsSource MTests/ColorizedPrint.cpp MTests/Starter.cpp MTests/GetLine.cpp MTests/TestLog.cpp)

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

project(VaultRecordRefUnitTest)

add_executable(VaultRecordRefUnitTest ${TestsHeaders} ${TestsSource} VaultRecordRefUnitTest.cpp)
target_link_libraries(VaultRecordRefUnitTest MVault)

project(VaultRecordRefMpTest)

add_executable(VaultRecordRefMpTest ${TestsHeaders} ${TestsSource} VaultRecordRefMpTest.cpp)
target_link_libraries(VaultRecordRefMpTest MVault)

project(VaultRecordSetMpTest)

add_executable(VaultRecordSetMpTest ${TestsHeaders} ${TestsSource} VaultRecordSetMpTest.cpp)
target_link_libraries(VaultRecordSetMpTest MVault)