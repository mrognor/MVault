cmake_minimum_required(VERSION 3.10)

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

    Strings/ToJson.h
    Strings/ToString.h
    Strings/FromString.h
    Strings/PrintAsTable.h

    BackTrace/BackTrace.h

    CsvParser/CsvParser.h
    CsvParser/StreamFileReader.h

    Uuid/Uuid.h
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

    Strings/ToJson.cpp

    BackTrace/BackTrace.cpp

    CsvParser/CsvParser.cpp
    CsvParser/StreamFileReader.cpp

    Uuid/Uuid.cpp
)

project(MVault)

add_library(MVault STATIC ${MVaultHeaders} ${MVaultTemplates} ${MVaultSource})