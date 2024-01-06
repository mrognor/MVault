set (DataStorageHeaders 
    DataStorage.h 
    DataStorageRecord.h 
    DataContainer.h 
    DataSaver.h 
    SmartPointerWrapper.h
    ReadWriteMutex.h)

set (DataStorageSource
    DataStorage.cpp 
    DataStorageRecord.cpp 
    DataSaver.cpp
    ReadWriteMutex.cpp)

project(DataStorage)

add_executable(DataStorage main.cpp ${DataStorageHeaders} ${DataStorageSource})