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

    std::cout << "Request interval [2, 4]" << std::endl;
    vrs.Clear();
    vlt.RequestInterval("A", 2, 4, vrs);
    vrs.PrintAsTable();

    std::cout << "Request interval [5, 9)" << std::endl;
    vrs.Clear();
    vlt.RequestInterval("A", 5, 9, vrs, true, false);
    vrs.PrintAsTable();

    std::cout << "Request interval (4, 7]" << std::endl;
    vrs.Clear();
    vlt.RequestInterval("A", 4, 7, vrs, false, true);
    vrs.PrintAsTable();

    std::cout << "Request interval (1, 6)" << std::endl;
    vrs.Clear();
    vlt.RequestInterval("A", 1, 6, vrs, false, false);
    vrs.PrintAsTable();

    std::cout << "Request interval [1, 9) and less 5 inside it" << std::endl;
    vrs.Clear();
    vlt.RequestInterval("A", 1, 9, vrs, true, false);

    mvlt::VaultRecordSet vrsReq;
    vrs.RequestLess("A", 5, vrsReq);
    vrsReq.PrintAsTable();

    std::cout << "Complex request A equal 2" << std::endl;
    vrs.Clear();
    vlt.Request(mvlt::Equal("A", 2), vrs);
    vrs.PrintAsTable();

    std::cout << "Complex request A greater 7" << std::endl;
    vrs.Clear();
    vlt.Request(mvlt::Greater("A", 7), vrs);
    vrs.PrintAsTable();
}