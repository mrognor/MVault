#include "Tests.h"
#include "ColorizedPrint.h"

int main()
{
    std::cout << "Testing has started!" << std::endl;

    ColorizedPrint("\tMultithreading tests: ", ConsoleTextColor::Cyan);
    
    MultithreadingReadTest();

    MultithreadingKeysTest();

    MultithreadingRecordsTest();

    std::cout << "\tThe multithreaded tests are finished!" << std::endl;

    ColorizedPrint("Testing is over!", ConsoleTextColor::Green);

    std::cout << "\tBench has started!" << std::endl;

    Bench();

    std::cout << "\tThe bench are finished!" << std::endl;

    ColorizedPrint("Bench is over!", ConsoleTextColor::Green);
}