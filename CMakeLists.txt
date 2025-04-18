cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

project(MVLT)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-Wextra -O3 -Wno-unused-parameter")
    add_compile_options(-D NDEBUG)
else()
    if(NOT WIN32)
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "-Wextra -fsanitize=address -fsanitize=leak -lgcov --coverage -g -Wno-unused-parameter")
        else()
            set(CMAKE_CXX_FLAGS "-Wextra -fsanitize=address -fsanitize=leak -g -Wno-unused-parameter")
        endif()
    endif()
endif()

add_subdirectory(Source)
add_subdirectory(Examples)
add_subdirectory(Tests)