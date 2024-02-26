cmake_minimum_required(VERSION 2.6)

if(WIN32)
    set(CMAKE_CXX_FLAGS "-Wall -g")
else()
    set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address  -fsanitize=leak -g")
endif()

set(CMAKE_CXX_STANDARD 11)

set(MVaultHeaders 
    Vault.h 
    VaultRecord.h
    VaultRecordRef.h
    VaultFunctions.h
    VaultOperationResult.h
    DataContainer.h 
    DataSaver.h 
    ReadWriteMutex.h
    Functions/ToString.h
    Functions/PrintAsTable.h)

set(MVaultSource
    Vault.cpp 
    VaultRecord.cpp
    VaultRecordRef.cpp
    VaultOperationResult.cpp
    DataSaver.cpp
    ReadWriteMutex.cpp)

project(MVault)

add_library(MVault STATIC ${MVaultHeaders} ${MVaultSource})