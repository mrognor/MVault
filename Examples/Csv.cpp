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

    vlt.SaveToFile("CsvResult.csv");
}
