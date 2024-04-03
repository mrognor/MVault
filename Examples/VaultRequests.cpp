#include "../Source/MVault.h"

int main()
{
    mvlt::Vault vlt;
    mvlt::VaultRecordSet vrs;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", -1);

    vlt.CreateRecord({ {"A", 0} });
    vlt.CreateRecord({ {"A", 9} });
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"B", 10 - i} });
    vlt.PrintAsTable();

    std::cout << "Request greater 5" << std::endl;
    vlt.RequestGreater("A", 5, vrs);
    vrs.PrintAsTable();

    std::cout << "Request 2 records greater or equal 2" << std::endl;
    vrs.Clear();
    vlt.RequestGreaterOrEqual("A", 2, vrs, 2);
    vrs.PrintAsTable();

    std::cout << "Request less or equal 2" << std::endl;
    vrs.Clear();
    vlt.RequestLessOrEqual("A", 2, vrs);
    vrs.PrintAsTable();

    std::cout << "Request less 5" << std::endl;
    vrs.Clear();
    vlt.RequestLess("A", 5, vrs);
    vrs.PrintAsTable();
}