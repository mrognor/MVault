#include "../Source/MVault.h"


int main()
{
    mvlt::Vault vlt;
    mvlt::VaultRecordRef vrr;

    vlt.AddKey("Key", -1);
    
    vlt.CreateRecord({ {"Key", 12} });

    // ============================================================================== //
    // ============================================================================== //

    std::cout << "GetRecordCheck:" << std::endl;

    // ============================================================================== //

    std::cout << "\tWrong key name" << std::endl;

    mvlt::VaultOperationResult opr = vlt.GetRecord("WrongKey", 1, vrr);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested key: " << opr.Key << std::endl;
    
    // ============================================================================== //

    std::cout << "\tWrong key type" << std::endl;

    opr = vlt.GetRecord("Key", std::string("12"), vrr);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested type: " << opr.RequestedType.name() << ". Saved type: " << opr.SavedType.name() << std::endl;

    // ============================================================================== //

    std::cout << "\tWrong key value" << std::endl;

    opr = vlt.GetRecord("Key", 11, vrr);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << std::endl;

    // ============================================================================== //

    std::cout << "\tCorrect request" << std::endl;
    opr = vlt.GetRecord("Key", 12, vrr);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested type: " << opr.RequestedType.name() << ". Saved type: " << opr.SavedType.name() << std::endl;
    else
        std::cout << "\t\tSuccess! Requested record: " << vrr.GetRecordUniqueId() << std::endl;

    // ============================================================================== //
    // ============================================================================== //

    vlt.CreateRecord({ {"Key", 12} });
    vlt.CreateRecord({ {"Key", 12} });

    std::cout << "GetRecordsCheck:" << std::endl;

    // ============================================================================== //

    std::cout << "\tWrong key name" << std::endl;

    std::vector<mvlt::VaultRecordRef> records;
    opr = vlt.GetRecords("WrongKey", 1, records);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested key: " << opr.Key << std::endl;
    
    // ============================================================================== //

    std::cout << "\tWrong key type" << std::endl;

    opr = vlt.GetRecords("Key", std::string("12"), records);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested type: " << opr.RequestedType.name() << ". Saved type: " << opr.SavedType.name() << std::endl;

    // ============================================================================== //

    std::cout << "\tWrong key value" << std::endl;

    opr = vlt.GetRecords("Key", 11, records);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << std::endl;

    // ============================================================================== //

    std::cout << "\tCorrect request" << std::endl;
    opr = vlt.GetRecords("Key", 12, records);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested type: " << opr.RequestedType.name() << ". Saved type: " << opr.SavedType.name() << std::endl;
    else
    {
        std::cout << "\t\tSuccess! Requested records: ";
        for (const auto& it : records) std::cout << it.GetRecordUniqueId() << " ";
        std::cout << std::endl;
    }

    // ============================================================================== //
    // ============================================================================== //

    std::cout << "CreateRecordCheck:" << std::endl;

    // ============================================================================== //

    std::cout << "\tWrong key name" << std::endl;

    opr = vlt.CreateRecord({ {"WrongKey", 1} });

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested key: " << opr.Key << std::endl;
    
    // ============================================================================== //

    std::cout << "\tWrong key type" << std::endl;

    opr = vlt.CreateRecord({ {"Key", std::string("12")} });

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested type: " << opr.RequestedType.name() << ". Saved type: " << opr.SavedType.name() << std::endl;

    // ============================================================================== //

    std::cout << "\tCorrect request" << std::endl;

    opr = vlt.CreateRecord({{"Key", 11}});

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested type: " << opr.RequestedType.name() << ". Saved type: " << opr.SavedType.name() << std::endl;
    else
        std::cout << "\t\tSuccess! Requested record: " << vrr.GetRecordUniqueId() << std::endl;

    // ============================================================================== //
    // ============================================================================== //

    std::cout << "VaultRecordRef::SetData:" << std::endl;

    vlt.CreateRecord(vrr, { {"Key", 333} });
    vlt.PrintAsTable();

    std::cout << vrr.GetRecordUniqueId() << std::endl;

    // ============================================================================== //

    std::cout << "\tWrong key name" << std::endl;

    opr = vrr.SetData("WrongKey", 121);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested key: " << opr.Key << std::endl;
    
    // ============================================================================== //

    std::cout << "\tWrong key type" << std::endl;

    opr = vrr.SetData("Key", std::string("121"));

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested type: " << opr.RequestedType.name() << ". Saved type: " << opr.SavedType.name() << std::endl;

    // ============================================================================== //

    std::cout << "\tCorrect request" << std::endl;

    opr = vrr.SetData("Key", 121);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested type: " << opr.RequestedType.name() << ". Saved type: " << opr.SavedType.name() << std::endl;
    else
        std::cout << "\t\tSuccess! Requested record: " << vrr.GetRecordUniqueId() << std::endl;

    vlt.PrintAsTable();

    // ============================================================================== //
    // ============================================================================== //

    std::cout << "VaultRecordRef::GetData:" << std::endl;

    // ============================================================================== //

    std::cout << "\tWrong key name" << std::endl;

    int i;
    opr = vrr.GetData("WrongKey", i);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested key: " << opr.Key << std::endl;
    
    // ============================================================================== //

    std::cout << "\tWrong key type" << std::endl;

    std::string s;
    opr = vrr.GetData("Key", s);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested type: " << opr.RequestedType.name() << ". Saved type: " << opr.SavedType.name() << std::endl;

    // ============================================================================== //

    std::cout << "\tCorrect request" << std::endl;

    opr = vrr.GetData("Key", i);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested type: " << opr.RequestedType.name() << ". Saved type: " << opr.SavedType.name() << std::endl;
    else
    {
        std::cout << "\t\tSuccess! Requested record: " << vrr.GetRecordUniqueId() << std::endl;
        std::cout << "\t\tRequested data: " << i << std::endl;
    }

    // ============================================================================== //

    std::cout << "\tInvalid record" << std::endl;

    vlt.EraseRecord(vrr);

    opr = vrr.GetData("Key", i);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << " Requested type: " << opr.RequestedType.name() << ". Saved type: " << opr.SavedType.name() << std::endl;
    else
        std::cout << "\t\tSuccess! Requested record: " << vrr.GetRecordUniqueId() << std::endl;

    // ============================================================================== //

    std::cout << "\tInvalid vault" << std::endl;

    mvlt::VaultRecordRef vrr2;

    opr = vrr2.GetData("key", i);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << std::endl;
    
    opr = vrr2.SetData("key", 777);

    if (!opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << std::endl;

    // ============================================================================== //

    std::cout << "\tDeleted vault" << std::endl;

    mvlt::Vault* vltp = new mvlt::Vault;
    vltp->AddKey("A", 1);

    vltp->CreateRecord();

    opr = vltp->GetRecord("A", 1, vrr);

    if (opr.IsOperationSuccess)
        std::cout << "\t\t" << opr.ResultCodeString() << std::endl;

    delete vltp;

    std::cout << vrr.IsValid() << " " << vrr.GetRecordUniqueId() << std::endl;

    std::cout << "Test ended" << std::endl;
}