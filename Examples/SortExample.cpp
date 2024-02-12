#include "../Source/Vault.h"

int main()
{
    mvlt::Vault vlt;

    vlt.SetKey("id", -1);
    vlt.SetKey<std::string>("name", "");
    vlt.SetKey<bool>("gender", true);

    vlt.CreateRecord({ {"id", 1}, {"name", std::string("Marfa")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 2}, {"name", std::string("Alexander")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 3}, {"name", std::string("John")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 4}, {"name", std::string("Patricia")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 5}, {"name", std::string("Mikhail")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 6}, {"name", std::string("Daria")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 7}, {"name", std::string("Natasha")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 8}, {"name", std::string("Jayson")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 9}, {"name", std::string("Alexander")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 10}, {"name", std::string("Varvara")}, {"gender", false} });

    std::cout << "Vault: " << std::endl;
    vlt.PrintAsTable();

    std::cout << "The first 5 records after sorting in ascending order by id: " << std::endl;
    std::vector<mvlt::VaultRecordRef> sorted = vlt.GetSortedRecords("id", false, 3);
    mvlt::PrintAsTable(sorted);
    
    std::cout << "The first 5 records without gender after sorting in descending order by name:" << std::endl;
    sorted = vlt.GetSortedRecords("name", true, 5);
    mvlt::PrintAsTable(sorted, -1, {"id", "name"});

    std::cout << "The record with the largest id with name Alexander: ";
    int greatestAlexanderId = -1;
    vlt.SortBy("id", [&](const mvlt::VaultRecordRef& recordRef)
        {
            std::string name;
            recordRef.GetData("name", name);
            if (name == "Alexander")
            {
                recordRef.GetData("id", greatestAlexanderId);
                return false;
            }
            else
                return true;
        });
    std::cout << greatestAlexanderId << std::endl;
}