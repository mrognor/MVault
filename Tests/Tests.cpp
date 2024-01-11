#include "Tests.h"

int main()
{
    std::cout << "Testing has started!" << std::endl;

    std::cout << "\tMultithreading tests: " << std::endl;
    
    MultithreadingReadTest();

    MultithreadingKeysTest();

    MultithreadingRecordsTest();

    std::cout << "\tThe multithreaded tests are finished!" << std::endl;

    ColorizedPrint("Testing is over!", ConsoleTextColor::Green);
}