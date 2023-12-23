HEADERS = 	DataStorage.h \
			DataStorageRecordSet.h \
			DataStorageRecord.h \
			DataContainer.h \
			DataSaver.h \
			SmartPointerWrapper.h \
			CsvParser.h \
			DataStorageRequests.h
SOURCES = 	DataStorage.cpp \
			DataStorageRecordSet.cpp \
			DataStorageRecord.cpp \
			DataSaver.cpp \
			CsvParser.cpp \
			DataStorageRequests.cpp

CXXFLAGS =-Wall -std=c++11
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
	g++ -g $(CXXFLAGS) main.cpp $(SOURCES) $(LDFLAGS) -o DataStorage$(FILEXT)

clean:
	rm DataStorage$(FILEXT)