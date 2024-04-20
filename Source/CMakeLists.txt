cmake_minimum_required(VERSION 3.6)

if(WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "-Wall -O3")
    else()
        set(CMAKE_CXX_FLAGS "-Wall -g")
    endif()
else()
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "-Wall -O3")
    else()
        set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address -fsanitize=leak -g")
    endif()
endif()

set(CMAKE_CXX_STANDARD 11)

set(MVaultHeaders
    MVault.h

    Vault.h 
    VaultRecord.h
    VaultRequest.h
    VaultRecordRef.h
    VaultFunctions.h
    VaultRecordSet.h
    VaultParamInput.h
    VaultOperationResult.h

    DataContainer.h 
    DataSaver.h 
    ReadWriteMutex.h

    Functions/ToString.h
    Functions/CsvParser.h
    Functions/PrintAsTable.h
    Functions/VaultRecordSetOperations.h
)

set(MVaultTemplates
    Vault.hpp
    VaultRequest.hpp
    VaultRecordRef.hpp
    VaultRecordSet.hpp
    VaultParamInput.hpp
)

set(MVaultSource
    Vault.cpp
    VaultRecord.cpp
    VaultRequest.cpp
    VaultRecordRef.cpp
    VaultParamInput.cpp
    VaultOperationResult.cpp

    DataSaver.cpp
    ReadWriteMutex.cpp
    VaultRecordSet.cpp
    
    Functions/CsvParser.cpp
    Functions/PrintAsTable.cpp
    Functions/VaultRecordSetOperations.cpp
)

project(MVault)

add_library(MVault STATIC ${MVaultHeaders} ${MVaultTemplates} ${MVaultSource})