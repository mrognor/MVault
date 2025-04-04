#include <MVault.h>

using namespace mvlt;

int main()
{
    Vault vlt;
    VaultRecordSet vrs1, vrs2, vrs3;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", -1);

    vlt.CreateRecord({ {"A", 0} });
    vlt.CreateRecord({ {"A", 9} });
    for (int i = 0; i < 10; ++i) vlt.CreateRecord({ {"A", i}, {"B", 10 - i} });
    vlt.Print();

    std::cout << "Request greater 5" << std::endl;
    vlt.RequestGreater("A", 5, vrs1);
    vrs1.Print();

    std::cout << "Request 2 records greater or equal 2" << std::endl;
    vrs1.Clear();
    vlt.RequestGreaterOrEqual("A", 2, vrs1, 2);
    vrs1.Print();

    std::cout << "Request less or equal 2" << std::endl;
    vrs1.Clear();
    vlt.RequestLessOrEqual("A", 2, vrs1);
    vrs1.Print();

    std::cout << "Request less 5" << std::endl;
    vrs1.Clear();
    vlt.RequestLess("A", 5, vrs1);
    vrs1.Print();

    std::cout << "Request interval [2, 4]" << std::endl;
    vrs1.Clear();
    vlt.RequestInterval("A", 2, 4, vrs1);
    vrs1.Print();

    std::cout << "Request interval [5, 9)" << std::endl;
    vrs1.Clear();
    vlt.RequestInterval("A", 5, 9, vrs1, true, false);
    vrs1.Print();

    std::cout << "Request interval (4, 7]" << std::endl;
    vrs1.Clear();
    vlt.RequestInterval("A", 4, 7, vrs1, false, true);
    vrs1.Print();

    std::cout << "Request interval (1, 6)" << std::endl;
    vrs1.Clear();
    vlt.RequestInterval("A", 1, 6, vrs1, false, false);
    vrs1.Print();

    std::cout << "Request interval [1, 9) and less 5 inside it" << std::endl;
    vrs1.Clear();
    vlt.RequestInterval("A", 1, 9, vrs1, true, false);

    vrs1.RequestLess("A", 5, vrs2);
    vrs2.Print();

    std::cout << "Join A < 2 and A > 7 and Exclude B < 5" << std::endl;
    vrs1.Clear();
    vrs2.Clear();

    vlt.Request(Less("A", 2), vrs1);
    vlt.Request(Greater("A", 7), vrs2);
    vlt.Request(Less("B", 5), vrs3);

    vrs1.Print();
    vrs2.Print();
    vrs3.Print();

    vrs1.Join(vrs2);
    vrs1.Print();
    vrs1.Exclude(vrs3);

    vrs1.Print();

    std::cout << "Complex requests" << std::endl;
    vlt.Request(Or(Less("A", 3), Greater("A", 7)), vrs1);
    vrs1.Print();

    vrs1.Clear();
    vlt.Request(And(Less("A", 3), Greater("B", 7)), vrs1);
    vrs1.Print();

    vrs1.Clear();
    vlt.Request(Or(And(Less("A", 3), Greater("B", 7)), Equal("A", 0)), vrs1);
    vrs1.Print();

    vrs1.Clear();
    vlt.Request(And(GreaterOrEqual("A", 3), Less("A", 7)), vrs1);
    vrs1.Print();

    vrs1.Clear();
    VaultOperationResult res = vlt.Request(And(GreaterOrEqual("Z", 3), Less("A", 7)), vrs1);
    if (!res.IsOperationSuccess)
        std::cout << res.ResultCodeString() << " Requested key: " << res.Key << std::endl;

    vrs1.Clear();
    vrs2.Clear();
    vrs3.Clear();
    vlt.Request(Equal("A", 0), vrs1);
    vlt.Request(Equal("A", 9), vrs2);
    Union(vrs1, vrs2, vrs3);
    vrs3.Print();

    vrs1.Clear();
    vrs2.Clear();
    vlt.Request(Equal("B", 1), vrs1);
    Intersection(vrs1, vrs3, vrs2);
    vrs2.Print();
    vrs1.Print();
    vrs3.Print();

    std::cout << "Predicat" << std::endl;
    vrs1.Clear();
    vlt.RequestEqual("A", 0, vrs1);
    vrs1.Print();

    vrs1.Clear();
    vlt.RequestEqual("A", 0, vrs1, -1, [](const VaultRecordRef& ref)
        {
            int B = 0;
            ref.GetData("B", B);
            if (B > 0)
                return true;
            else
                return false;
        });

    vrs1.Print();

    vrs1.Clear();
    vlt.Request(Greater("A", 3, [](const VaultRecordRef& ref)
        {
            int B = 0;
            ref.GetData("B", B);
            if (B != 6)
                return true;
            else
                return false;
        }) && Less("A", 7, [](const VaultRecordRef& ref)
        {
            int B = 0;
            ref.GetData("B", B);
            if (B != 4)
                return true;
            else
                return false;
        }), vrs1);
    vrs1.Print();

    std::cout << "Files" << std::endl;

    vlt.DropVault();

    vlt.AddKey<std::string>("cpuName", "");
    vlt.AddKey<float>("price", 0.0);
    vlt.AddKey<int>("cpuMark", 0);
    vlt.AddKey<float>("cpuValue", 0.0);
    vlt.AddKey<int>("threadMark", 0);
    vlt.AddKey<float>("threadValue", 0.0);
    vlt.AddKey<int>("TDP", 0);
    vlt.AddKey<float>("powerPerf", 0.0);
    vlt.AddKey<int>("cores", 0);
    vlt.AddKey<int>("testDate", 0);
    vlt.AddKey<std::string>("socket", "");
    vlt.AddKey<std::string>("category", "");
    
    vlt.ReadFile("Csv/CPU_benchmark_v4.csv");

    vrs1.Clear();
    vlt.RequestGreater("cpuMark", 80000, vrs1);
    vrs1.SaveToFile("RequestRes.txt");
}