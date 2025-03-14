#include <MVault.h>

int main()
{
    mvlt::Vault vlt;
    
    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    vlt.CreateRecord({});
    vlt.CreateRecord({ {"A", 1} });
    vlt.CreateRecord({ {"B", 1} });

    std::cout << "Origin vault: " << std::endl;
    vlt.Print(true);


    mvlt::VaultRecordSet vrs, vrs2;
    vlt.RequestEqual("A", 0, vrs);
    vrs2 = vrs;
    std::cout << "Requested vault: " << std::endl;
    vrs.Print(true);

    std::cout << "Get record: " << std::endl;

    mvlt::VaultRecordRef vrr;
    vrs.GetRecord("B", 1, vrr);
    vrr.PrintRecord();

    std::cout << "Chanhing value in requested vault" << std::endl;
    vrr.SetData("A", 2);

    std::cout << "Origin vault: " << std::endl;
    vlt.Print(true);

    std::cout << "Requested vault: " << std::endl;
    vrs.Print(true);

    std::cout << "Copied vault: " << std::endl;
    vrs2.Print(true);

    std::cout << "Subrequest" << std::endl;
    vrs.RequestEqual("A", 2, vrs2);
    vrs2.Print(true);

    vrr.SetData("B", 3);
    std::cout << "SetData in ref" << std::endl;
    vlt.Print(true);
    vrs.Print(true);
    vrs2.Print(true);

    std::cout << "Add record to RecordSet" << std::endl;
    vlt.GetRecord("A", 1, vrr);
    vrs2.AddRecord(vrr);
    vrs2.Print(true);

    std::cout << "Erase record from vault. 1" << std::endl;
    std::cout << "Before:" << std::endl;
    vlt.Print(true);
    vrs.Print(true);

    vrs2.GetRecord("A", 2, vrr);
    vlt.EraseRecord(vrr);

    std::cout << "After:" << std::endl;
    vlt.Print(true);
    vrs.Print(true);

    std::cout << "Erase record from vault. 2" << std::endl;
    std::cout << "Before:" << std::endl;
    vlt.Print(true);
    vrs.Print(true);

    vlt.EraseRecord("A", 0);

    std::cout << "After:" << std::endl;
    vlt.Print(true);
    vrs.Print(true);

    std::cout << "Drop vault" << std::endl;
    vlt.DropData();
    vrs.Print();
    
    std::cout << "Clear and reset" << std::endl;
    vrs.Clear();
    vrs.Print();
    vrs2.Reset();
    vrs2.Print();

    std::cout << "Erase records" << std::endl;

    vlt.CreateRecord({});
    vlt.CreateRecord({});
    vlt.CreateRecord({});
    
    vlt.RequestEqual("A", 0, vrs);

    std::cout << "Before" << std::endl;
    vlt.Print(true);
    vrs.Print(true);

    std::cout << "After" << std::endl;
    
    vlt.EraseRecords("A", 0);
    vlt.Print(true);
    vrs.Print(true);

    std::cout << "Adding keys" << std::endl;
    std::cout << "Before" << std::endl; 
    vlt.Print();
    vrs.Print();
    
    std::cout << "After 1" << std::endl;
    vlt.AddKey("C", -1);
    vlt.Print();
    vrs.Print();

    std::cout << "After 2" << std::endl;
    vlt.RemoveKey("B");
    vlt.Print();
    vrs.Print();
}