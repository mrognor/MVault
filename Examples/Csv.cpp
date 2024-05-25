#include "../Source/MVault.h"

int main()
{
    std::cout << mvlt::ToString(float(7299.99)) << std::endl;
    std::cout << std::to_string(float(7299.99)) << std::endl;
    mvlt::Vault vlt;

    vlt.AddKey("id", -1);    
    vlt.AddKey<std::string>("name", "");
    vlt.AddKey("gender", true);

    vlt.CreateRecord({ {"id", 0}, {"name", std::string("A")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 1}, {"name", std::string("B")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 2}, {"name", std::string("C")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 3}, {"name", std::string("D")}, {"gender", false} });
    vlt.PrintAsTable();

    vlt.SaveToFile("CsvResult.csv", ";", false);
    vlt.DropVault();


    vlt.AddKey("cpuName", std::string());    
    vlt.AddKey<float>("price", 0.0);
    vlt.AddKey("cpuMark", 0);
    vlt.AddKey<float>("cpuValue", 0.0);    
    vlt.AddKey("threadMark", 0);
    vlt.AddKey<float>("threadValue", 0.0);
    vlt.AddKey("TDP", 0);    
    vlt.AddKey<float>("powerPerf", 0.0);
    vlt.AddKey("cores", 0);
    vlt.AddKey("testDate", 0);
    vlt.AddKey("socket", std::string());
    vlt.AddKey("category", std::string());


    vlt.ReadFile("CPU_benchmark_v4.csv", ',', true);
    vlt.PrintAsTable(false, 10, "cpuMark", true);
}
