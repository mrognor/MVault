#include "../Source/Vault.h"


int main()
{
    mvlt::Vault vlt;

    vlt.SetKey("Key", -1);
    
    vlt.CreateRecord({ {"Key", 12} });

    std::cout << "GetRecordCheck:" << std::endl;

    // ============================================================================== //

    std::cout << "\tWrong key name" << std::endl;
    
    mvlt::VaultRecordRef vrr;
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

    std::cout << "Z" << std::endl;
}