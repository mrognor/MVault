all: DataStorage

DataStorage: DataSaver.h DataSaver.cpp DataStorage.cpp SmartPointerWrapper.h DataStorageRecord.h DataStorageRecord.cpp DataStorageRecordSet.h DataStorageRecordSet.cpp
	g++ -g DataSaver.cpp DataStorage.cpp -fsanitize=address -o DataStorage DataStorageRecord.cpp DataStorageRecordSet.cpp

clean:
	rm DataStorage