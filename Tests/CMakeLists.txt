cmake_minimum_required(VERSION 3.10)

if(WIN32)
    set(CMAKE_CXX_FLAGS "-Wextra -g")
else()
    set(CMAKE_CXX_FLAGS "-Wextra -fsanitize=address -fsanitize=leak -lgcov --coverage -g -Wno-unused-parameter")
endif()

set(CMAKE_CXX_STANDARD 11)

set(MTestsHeaders MTests/ColorizedPrint.h MTests/TestAssert.h MTests/Starter.h MTests/GetLine.h MTests/TestLog.h)

set(MTestsSource MTests/ColorizedPrint.cpp MTests/Starter.cpp MTests/GetLine.cpp MTests/TestLog.cpp)

project(MTests)

add_library(MTests STATIC ${MTestsHeaders} ${MTestsSource})


include_directories(../Source)


project(Scenarios)

add_executable(Scenarios Scenarios.cpp)
target_link_libraries(Scenarios MVault)
target_link_libraries(Scenarios MTests)


project(VaultUnitTest)

add_executable(VaultUnitTest VaultUnitTest.cpp)
target_link_libraries(VaultUnitTest MVault)
target_link_libraries(VaultUnitTest MTests)


project(VaultRecordSetUnitTest)

add_executable(VaultRecordSetUnitTest VaultRecordSetUnitTest.cpp)
target_link_libraries(VaultRecordSetUnitTest MVault)
target_link_libraries(VaultRecordSetUnitTest MTests)


project(VaultRecordRefUnitTest)

add_executable(VaultRecordRefUnitTest VaultRecordRefUnitTest.cpp)
target_link_libraries(VaultRecordRefUnitTest MVault)
target_link_libraries(VaultRecordRefUnitTest MTests)


project(VaultRecordRefMpTest)

add_executable(VaultRecordRefMpTest VaultRecordRefMpTest.cpp)
target_link_libraries(VaultRecordRefMpTest MVault)
target_link_libraries(VaultRecordRefMpTest MTests)


project(VaultRecordSetMpTest)

add_executable(VaultRecordSetMpTest VaultRecordSetMpTest.cpp)
target_link_libraries(VaultRecordSetMpTest MVault)
target_link_libraries(VaultRecordSetMpTest MTests)
