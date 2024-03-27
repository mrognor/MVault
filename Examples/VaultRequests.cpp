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
    vlt.PrintAsTable(true);


    mvlt::VaultRecordSet vrer, vrer2;
    vlt.RequestRecords("A", 0, vrer);
    vrer2 = vrer;
    std::cout << "Requested vault: " << std::endl;
    vrer.PrintAsTable(true);

    std::cout << "Get record: " << std::endl;

    mvlt::VaultRecordRef vaultRecordRef;
    vrer.GetRecord("B", 1, vaultRecordRef);
    vaultRecordRef.PrintRecord();

    std::cout << "Chanhing value in requested vault" << std::endl;
    vaultRecordRef.SetData("A", 2);

    std::cout << "Origin vault: " << std::endl;
    vlt.PrintAsTable(true);

    std::cout << "Requested vault: " << std::endl;
    vrer.PrintAsTable(true);

    std::cout << "Copied vault: " << std::endl;
    vrer2.PrintAsTable(true);

    std::cout << "Subrequest" << std::endl;
    vrer.RequestRecords("A", 2, vrer2);
    vrer2.PrintAsTable(true);

    vaultRecordRef.SetData("B", 3);
    std::cout << "SetData in ref" << std::endl;
    vlt.PrintAsTable(true);
    vrer.PrintAsTable(true);
    vrer2.PrintAsTable(true);

    std::cout << "Add record to RecordSet" << std::endl;
    vlt.GetRecord("A", 1, vaultRecordRef);
    vrer2.AddRecord(vaultRecordRef);
    vrer2.PrintAsTable(true);

    std::cout << "Clear and reset" << std::endl;
    vrer.Clear();
    vrer.PrintAsTable();
    vrer2.Reset();
    vrer2.PrintAsTable();
}