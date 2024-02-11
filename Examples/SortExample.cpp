#include "../Source/Vault.h"

int main()
{
    mvlt::Vault vlt;

    vlt.SetKey("id", -1);
    vlt.SetKey("rid", -1);

    for (int i = 0; i < 20; ++i)
        vlt.CreateRecord({ {"id", i}, {"rid", 20 - i} });
    
    vlt.PrintAsTable();

    std::cout << "The first 5 entries after sorting in ascending order by id" << std::endl;
    std::vector<mvlt::VaultRecordRef> sorted = vlt.GetSortedRecords("id", false, 5);
    mvlt::PrintAsTable(sorted);
    
    std::cout << "The first 5 entries id after sorting in descending order by id" << std::endl;
    sorted = vlt.GetSortedRecords("id", true, 5);
    mvlt::PrintAsTable(sorted, -1, {"id"});

    std::cout << "The first 5 entries after sorting in ascending order by rid" << std::endl;
    sorted = vlt.GetSortedRecords("rid", false, 5);
    mvlt::PrintAsTable(sorted);
}