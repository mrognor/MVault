cmake_minimum_required(VERSION 3.10)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-Wextra -O3")
    add_compile_options(-D NDEBUG)
else()
    if(WIN32)
        set(CMAKE_CXX_FLAGS "-Wextra -g")
    else()
        set(CMAKE_CXX_FLAGS "-Wextra -fsanitize=address -fsanitize=leak -lgcov --coverage -g -Wno-unused-parameter")
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
    Functions/StreamFileReader.h
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
    Functions/StreamFileReader.cpp
)

project(MVault)

add_library(MVault STATIC ${MVaultHeaders} ${MVaultTemplates} ${MVaultSource})