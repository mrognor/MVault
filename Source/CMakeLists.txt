cmake_minimum_required(VERSION 3.6)

if(WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "-Wextra -O3")
    else()
        set(CMAKE_CXX_FLAGS "-Wextra -g")
    endif()
else()
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "-Wextra -O3")
    else()
        set(CMAKE_CXX_FLAGS "-Wextra -fsanitize=address -fsanitize=leak -lgcov --coverage -g")
    endif()
endif()

set(CMAKE_CXX_STANDARD 11)

set(MVaultHeaders
    MVault.h

    Map.h
    Vault.h
    VaultRecord.h
    VaultRequest.h
    VaultRecordRef.h
    VaultRecordSet.h
    VaultParamInput.h
    VaultOperationResult.h

    DataContainer.h
    DataSaver.h
    ReadWriteMutex.h

    Functions/ToString.h
    Functions/BackTrace.h
    Functions/CsvParser.h
    Functions/FromString.h
    Functions/PrintAsTable.h
)

set(MVaultTemplates
    Vault.hpp
    VaultRecord.hpp
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
    Functions/BackTrace.cpp
)

project(MVault)

add_library(MVault STATIC ${MVaultHeaders} ${MVaultTemplates} ${MVaultSource})