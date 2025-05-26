cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

project(Examples)

include_directories(../Source)	

add_executable(Basics Basics.cpp)
target_link_libraries(Basics MVault)

add_executable(Keys Keys.cpp)
target_link_libraries(Keys MVault)

add_executable(Requests Requests.cpp)
target_link_libraries(Requests MVault)

add_executable(Sets Sets.cpp)
target_link_libraries(Sets MVault)

add_executable(CustomType CustomType.cpp)
target_link_libraries(CustomType MVault)

add_executable(ErrorHandling ErrorHandling.cpp)
target_link_libraries(ErrorHandling MVault)

add_executable(Sorting ErrorHandling.cpp)
target_link_libraries(Sorting MVault)
