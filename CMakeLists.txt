if(WIN32)
    set(CMAKE_CXX_FLAGS "-Wall -g")
else()
    set(CMAKE_CXX_FLAGS "-Wall -fsanitize=address  -fsanitize=leak -g")
endif()

set (CMAKE_CXX_STANDARD 11)
set(SOURCE_DIR Source)

set (DataStorageHeaders 
    ${SOURCE_DIR}/DataStorage.h 
    ${SOURCE_DIR}/DataStorageRecord.h 
    ${SOURCE_DIR}/DataContainer.h 
    ${SOURCE_DIR}/DataSaver.h 
    ${SOURCE_DIR}/ReadWriteMutex.h)

set (DataStorageSource
    ${SOURCE_DIR}/DataStorage.cpp 
    ${SOURCE_DIR}/DataStorageRecord.cpp 
    ${SOURCE_DIR}/DataSaver.cpp
    ${SOURCE_DIR}/ReadWriteMutex.cpp)

project(DataStorage)

add_executable(DataStorage ${SOURCE_DIR}/main.cpp ${DataStorageHeaders} ${DataStorageSource})


set(SOURCE_DIR Tests)

set (TestsSource
    ${SOURCE_DIR}/ColorizedPrint.cpp
    ${SOURCE_DIR}/MultithreadingTest.cpp
    ${SOURCE_DIR}/Bench.cpp)

project(Test)

add_executable(Test ${SOURCE_DIR}/Tests.cpp ${DataStorageHeaders} ${DataStorageSource} ${TestsSource})
