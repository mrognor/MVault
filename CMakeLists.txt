project(DataStorage)

set (Headers DataSaver.h DataContainer.h DataStorageRecord.h DataStorageRecordSet.h)
set (Sources DataStorage.cpp DataSaver.cpp DataStorageRecord.cpp DataStorageRecordSet.cpp)

add_executable(DataStorage ${Headers} ${Sources})