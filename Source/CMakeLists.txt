cmake_minimum_required(VERSION 3.28)

if(WIN32)
    set(CMAKE_CXX_FLAGS "-Wall -g")
else()
    set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address  -fsanitize=leak -g")
endif()

set(CMAKE_CXX_STANDARD 11)

set(MVaultHeaders 
    Vault.h 
    VaultRecord.h 
    DataContainer.h 
    DataSaver.h 
    ReadWriteMutex.h
    ToString.h)

set(MVaultSource
    Vault.cpp 
    VaultRecord.cpp 
    DataSaver.cpp
    ReadWriteMutex.cpp)

project(MVault)

add_library(MVault STATIC ${MVaultHeaders} ${MVaultSource})