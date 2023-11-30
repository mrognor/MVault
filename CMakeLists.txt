project(DataStorage)

set (Headers DataSaver.h DataContainer.h)
set (Sources DataStorage.cpp DataSaver.cpp)

add_executable(DataStorage ${Headers} ${Sources})