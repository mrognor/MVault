cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

set(MTestsHeaders MTests/ColorizedPrint.h MTests/TestAssert.h MTests/Starter.h MTests/GetLine.h MTests/TestLog.h MTests/TestSuite.h)

set(MTestsSource MTests/ColorizedPrint.cpp MTests/Starter.cpp MTests/GetLine.cpp MTests/TestLog.cpp)

project(MTests)

add_library(MTests STATIC ${MTestsHeaders} ${MTestsSource})


include_directories(../Source)

set(UnitTestsHeaders UnitTests/VaultUnitTests.h UnitTests/VaultRecordSetUnitTests.h)

set(UnitTestsSource UnitTests/VaultUnitTests.cpp UnitTests/VaultRecordSetUnitTests.cpp)

project(UnitTests)

add_executable(UnitTests UnitTests/main.cpp ${UnitTestsHeaders} ${UnitTestsSource})
target_link_libraries(UnitTests MVault)
target_link_libraries(UnitTests MTests)
