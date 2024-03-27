cmake_minimum_required(VERSION 3.6)

if(WIN32)
    set(CMAKE_CXX_FLAGS "-Wall -g")
else()
    set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address  -fsanitize=leak -g")
endif()

set(CMAKE_CXX_STANDARD 11)

set(MVaultHeaders 
    MVault.h
    Vault.h 
    VaultRecord.h
    VaultRecordRef.h
    VaultFunctions.h
    VaultOperationResult.h
    VaultRecordSet.h
    VaultParamInput.h
    DataContainer.h 
    DataSaver.h 
    ReadWriteMutex.h
    Functions/ToString.h
    Functions/PrintAsTable.h
    Vault.hpp
    VaultRecordRef.hpp
    VaultParamInput.hpp)

set(MVaultSource
    Vault.cpp 
    VaultRecord.cpp
    VaultRecordRef.cpp
    VaultOperationResult.cpp
    VaultParamInput.cpp
    DataSaver.cpp
    ReadWriteMutex.cpp
    VaultRecordSet.cpp
    Functions/PrintAsTable.cpp)

project(MVault)

add_library(MVault STATIC ${MVaultHeaders} ${MVaultSource})