cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

set(MTestsHeaders
    MTests/Starter.h
    MTests/GetLine.h
    MTests/TestLog.h
    MTests/TestCout.h
    MTests/TestSuite.h
    MTests/TestAssert.h
    MTests/TypeWrapper.h
    MTests/CompareFile.h
    MTests/CompareVault.h
    MTests/ColorizedPrint.h
    MTests/CompareRequest.h
    MTests/CompareVaultRecordRef.h
    MTests/CompareVaultOperationResult.h)

set(MTestsSource
    MTests/Starter.cpp
    MTests/GetLine.cpp
    MTests/TestLog.cpp
    MTests/ColorizedPrint.cpp
    MTests/CompareVaultRecordRef.cpp)

project(MTests)

add_library(MTests STATIC ${MTestsHeaders} ${MTestsSource})


include_directories(../Source)

set(UnitTestsHeaders UnitTests/VaultUnitTests.h UnitTests/VaultRecordSetUnitTests.h)

set(UnitTestsSource UnitTests/VaultUnitTests.cpp UnitTests/VaultRecordSetUnitTests.cpp)

project(UnitTests)

add_executable(UnitTests UnitTests/main.cpp ${UnitTestsHeaders} ${UnitTestsSource})
target_link_libraries(UnitTests MVault)
target_link_libraries(UnitTests MTests)
