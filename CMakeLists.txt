project(DataStorage)

set (Headers DataSaver.h DataContainer.h DataStorageRecord.h)
set (Sources DataStorage.cpp DataSaver.cpp DataStorageRecord.cpp)

add_executable(DataStorage ${Headers} ${Sources})