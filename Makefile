all: DataStorage

DataStorage: DataSaver.h DataSaver.cpp DataStorage.cpp
	g++ -g DataSaver.cpp DataStorage.cpp -fsanitize=address -o DataStorage