HEADERS = DataStorage.h DataStorageRecordSet.h DataStorageRecord.h DataContainer.h DataSaver.h SmartPointerWrapper.h CsvParser.h
SOURCES = DataStorage.cpp DataStorageRecordSet.cpp DataStorageRecord.cpp DataSaver.cpp CsvParser.cpp

LDFLAGS =
FILEXT =

ifeq ($(OS), Windows_NT)
	FILEXT +=.exe
endif

ifneq ($(OS), Windows_NT)
	LDFLAGS +=-fsanitize=address
endif

all: DataStorage$(FILEXT)

DataStorage$(FILEXT): $(HEADERS) $(SOURCES) main.cpp
	g++ -g main.cpp $(SOURCES) $(LDFLAGS) -o DataStorage$(FILEXT)

clean:
	rm DataStorage$(FILEXT)