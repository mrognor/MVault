HEADERS = DataStorage.h DataStorageRecordSet.h DataStorageRecord.h DataContainer.h DataSaver.h SmartPointerWrapper.h
SOURCES = DataStorage.cpp DataStorageRecordSet.cpp DataStorageRecord.cpp DataSaver.cpp

all: DataStorage

DataStorage: $(HEADERS) $(SOURCES) main.cpp
	g++ -g main.cpp $(SOURCES) -fsanitize=address -o DataStorage

clean:
	rm DataStorage