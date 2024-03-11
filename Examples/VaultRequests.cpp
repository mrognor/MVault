#include "../Source/MVault.h"

int main()
{
    mvlt::Vault vlt, requestedVlt;
    
    vlt.SetKey("A", 0);
    vlt.SetKey("B", 0);

    vlt.CreateRecord();
    vlt.CreateRecord({ {"A", 1} });
    vlt.CreateRecord({ {"B", 1} });

    std::cout << "Origin vault: " << std::endl;
    vlt.PrintAsTable();

    vlt.RequestRecords("A", 0, requestedVlt);
    std::cout << "Requested vault: " << std::endl;
    requestedVlt.PrintAsTable();
}