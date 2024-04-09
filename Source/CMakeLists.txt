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
    Functions/PrintAsTable.h
)

set(MVaultTemplates
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
    
    Functions/PrintAsTable.cpp
)

project(MVault)

add_library(MVault STATIC ${MVaultHeaders} ${MVaultTemplates} ${MVaultSource})