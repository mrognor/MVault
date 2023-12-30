set (DataStorageHeaders 
    DataStorage.h 
    DataStorageRecord.h 
    DataContainer.h 
    DataSaver.h 
    SmartPointerWrapper.h)

set (DataStorageSource
    DataStorage.cpp 
    DataStorageRecord.cpp 
    DataSaver.cpp)

project(DataStorage)

add_executable(DataStorage main.cpp ${DataStorageHeaders} ${DataStorageSource})