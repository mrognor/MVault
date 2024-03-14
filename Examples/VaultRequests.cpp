#include "../Source/MVault.h"

int main()
{
    mvlt::Vault vlt;
    
    vlt.SetKey("A", 0);
    vlt.SetKey("B", 0);

    vlt.CreateRecord();
    vlt.CreateRecord({ {"A", 1} });
    vlt.CreateRecord({ {"B", 1} });

    std::cout << "Origin vault: " << std::endl;
    vlt.PrintVault();


    mvlt::VaultRequestResult vrer;
    vlt.RequestRecords("A", 0, vrer);
    std::cout << "Requested vault: " << std::endl;
    vrer.PrintVault();

    std::cout << "Get record: " << std::endl;

    mvlt::VaultRecordRef vaultRecordRef;
    vrer.GetRecord("B", 1, vaultRecordRef);
    vaultRecordRef.PrintRecord();

    std::cout << "Chanhing value in requested vault" << std::endl;
    vaultRecordRef.SetData("A", 2);

    std::cout << "Origin vault: " << std::endl;
    vlt.PrintVault();

    std::cout << "Requested vault: " << std::endl;
    vrer.PrintVault();

    std::cout << "Get record from VaultRequestResult: " << std::endl;

    vrer.GetRecord("A", 2, vaultRecordRef);
    vaultRecordRef.PrintRecord();

    std::cout << "Get record from Vault: " << std::endl;

    vlt.GetRecord("A", 2, vaultRecordRef);
    vaultRecordRef.PrintRecord();

    std::cout << "Z" << std::endl;
}