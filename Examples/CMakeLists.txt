cmake_minimum_required(VERSION 2.6)

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

add_executable(CustomTypeExample CustomTypeExample.cpp)
target_link_libraries(CustomTypeExample MVault)

add_executable(HandlingErrors HandlingErrors.cpp)
target_link_libraries(HandlingErrors MVault)

add_executable(VaultRequests VaultRequests.cpp)
target_link_libraries(VaultRequests MVault)