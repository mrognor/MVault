cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)

project(MVLT)

try_compile(BACKTRACE_SUPPORT "${CMAKE_BINARY_DIR}/Build" "${CMAKE_SOURCE_DIR}/Checkers/BackTrace.cpp")

if (NOT ${BACKTRACE_SUPPORT})
    add_compile_definitions(NO_BACKTRACE_SUPPORT)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-Wextra -O2 -Wno-unused-parameter")
    add_compile_definitions(NDEBUG)
else()
    if(WIN32)
        if (CMAKE_GENERATOR MATCHES "Visual Studio")
        set(CMAKE_CXX_FLAGS "/Wall /bigobj")
        else()
            set(CMAKE_CXX_FLAGS "-Wextra -g -Wno-unused-parameter -Wa,-mbig-obj")
        endif()
    endif()

    if(LINUX)
        set(CMAKE_CXX_FLAGS "-fsanitize=address -fsanitize=leak")

        try_compile(ASAN_SUPPORT "${CMAKE_BINARY_DIR}/Build" "${CMAKE_SOURCE_DIR}/Checkers/AsanChecker.cpp")

        unset(CMAKE_CXX_FLAGS)

        if (${ASAN_SUPPORT})
            add_compile_options(-fsanitize=address)
            add_link_options(-fsanitize=address)
            add_compile_options(-fsanitize=leak)
            add_link_options(-fsanitize=leak)
        endif()

        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            set(CMAKE_CXX_FLAGS "-Wextra -lgcov --coverage -g -Wno-unused-parameter")
        else()
            set(CMAKE_CXX_FLAGS "-Wextra -g -Wno-unused-parameter")
        endif()
    endif()
endif()

add_subdirectory(Source)
add_subdirectory(Examples)


if (NOT "$ENV{CI}" STREQUAL "")
    add_subdirectory(Tests)
    add_subdirectory(Benchmarks)
endif()