#include "../Source/Vault.h"


int main()
{
    mvlt::Vault vlt;
    mvlt::VaultRecordRef vrr;

    vlt.SetKey("Key", -1);
    
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

    std::cout << "Z" << std::endl;
}