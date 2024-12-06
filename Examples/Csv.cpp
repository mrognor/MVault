#include "../Source/MVault.h"

int main()
{
    mvlt::Vault vlt;

    vlt.AddKey("id", -1);    
    vlt.AddKey<std::string>("name", "");
    vlt.AddKey("gender", true);

    vlt.CreateRecord({ {"id", 0}, {"name", std::string("A")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 1}, {"name", std::string("B")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 2}, {"name", std::string("C")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 3}, {"name", std::string("D")}, {"gender", false} });
    vlt.PrintAsTable();

    vlt.SaveToFile("CsvResult.csv", {}, ";");
    vlt.DropVault();

    vlt.AddKey("id", -1);    
    vlt.AddKey<std::string>("name", "");
    vlt.AddKey<std::string>("gender", "");

    vlt.ReadFile("CsvResult.csv", ';', true, [](const std::vector<std::string>& keys, std::vector<std::string>& values) 
    {
        if (values[2] == "true") values[2] = "Man";
        else if (values[2] == "false") values[2] = "Woman";
    });

    vlt.PrintAsTable();
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


    vlt.ReadFile("Csv/CPU_benchmark_v4.csv", ',', true);
    vlt.PrintAsTable(false, 10, "cpuMark", true);
}
