cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

project(Examples)

include_directories(../Source)	

add_executable(Example1 Example1.cpp)
target_link_libraries(Example1 MVault)

add_executable(SortExample SortExample.cpp)
target_link_libraries(SortExample MVault)

add_executable(CustomTypeExample CustomTypeExample.cpp)
target_link_libraries(CustomTypeExample MVault)

add_executable(HandlingErrors HandlingErrors.cpp)
target_link_libraries(HandlingErrors MVault)

add_executable(VaultRecordSet VaultRecordSet.cpp)
target_link_libraries(VaultRecordSet MVault)

add_executable(VaultRequests VaultRequests.cpp)
target_link_libraries(VaultRequests MVault)

add_executable(Csv Csv.cpp)
target_link_libraries(Csv MVault)

add_executable(DebugPrint DebugPrint.cpp)
target_link_libraries(DebugPrint MVault)