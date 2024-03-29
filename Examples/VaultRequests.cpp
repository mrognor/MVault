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


    mvlt::VaultRecordSet vrs, vrs2;
    vlt.RequestRecords("A", 0, vrs);
    vrs2 = vrs;
    std::cout << "Requested vault: " << std::endl;
    vrs.PrintAsTable(true);

    std::cout << "Get record: " << std::endl;

    mvlt::VaultRecordRef vrr;
    vrs.GetRecord("B", 1, vrr);
    vrr.PrintRecord();

    std::cout << "Chanhing value in requested vault" << std::endl;
    vrr.SetData("A", 2);

    std::cout << "Origin vault: " << std::endl;
    vlt.PrintAsTable(true);

    std::cout << "Requested vault: " << std::endl;
    vrs.PrintAsTable(true);

    std::cout << "Copied vault: " << std::endl;
    vrs2.PrintAsTable(true);

    std::cout << "Subrequest" << std::endl;
    vrs.RequestRecords("A", 2, vrs2);
    vrs2.PrintAsTable(true);

    vrr.SetData("B", 3);
    std::cout << "SetData in ref" << std::endl;
    vlt.PrintAsTable(true);
    vrs.PrintAsTable(true);
    vrs2.PrintAsTable(true);

    std::cout << "Add record to RecordSet" << std::endl;
    vlt.GetRecord("A", 1, vrr);
    vrs2.AddRecord(vrr);
    vrs2.PrintAsTable(true);

    std::cout << "Erase record from vault. 1" << std::endl;
    std::cout << "Before:" << std::endl;
    vlt.PrintAsTable(true);
    vrs.PrintAsTable(true);

    vrs2.GetRecord("A", 2, vrr);
    vlt.EraseRecord(vrr);

    std::cout << "After:" << std::endl;
    vlt.PrintAsTable(true);
    vrs.PrintAsTable(true);

    std::cout << "Erase record from vault. 2" << std::endl;
    std::cout << "Before:" << std::endl;
    vlt.PrintAsTable(true);
    vrs.PrintAsTable(true);

    vlt.EraseRecord("A", 0);

    std::cout << "After:" << std::endl;
    vlt.PrintAsTable(true);
    vrs.PrintAsTable(true);

    std::cout << "Drop vault" << std::endl;
    vlt.DropData();
    vrs.PrintAsTable();
    
    std::cout << "Clear and reset" << std::endl;
    vrs.Clear();
    vrs.PrintAsTable();
    vrs2.Reset();
    vrs2.PrintAsTable();

    std::cout << "Erase records" << std::endl;

    vlt.CreateRecord();
    vlt.CreateRecord();
    vlt.CreateRecord();
    
    vlt.RequestRecords("A", 0, vrs);

    std::cout << "Before" << std::endl;
    vlt.PrintAsTable(true);
    vrs.PrintAsTable(true);

    std::cout << "After" << std::endl;
    
    vlt.EraseRecords("A", 0);
    vlt.PrintAsTable(true);
    vrs.PrintAsTable(true);
}