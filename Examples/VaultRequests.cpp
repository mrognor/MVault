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

    std::cout << "Chanhing value in requested vault" << std::endl;

    mvlt::VaultRecordRef vrr;
    requestedVlt.GetRecord("B", 1, vrr);
    vrr.SetData("A", 2);

    std::cout << "Origin vault: " << std::endl;
    vlt.PrintAsTable();

    std::cout << "Requested vault: " << std::endl;
    requestedVlt.PrintAsTable();
}