/*
    This file provides an example of basic library usage.
    In this example, a table with 4 keys will be used.:
    1. The name of the person
    2. City of residence
    3. Gender. True - male, False - female
    4. Year of birth
*/

#include <MVault.h>

int main()
{
    // Create vault to store data
    mvlt::Vault vlt;

    // Add keys to vault
    vlt.AddKey<std::string>("Name", "");
    vlt.AddKey<std::string>("City", "");
    vlt.AddKey<bool>("Gender", true);
    vlt.AddKey<std::uint16_t>("Birth Year", 0);


    // Create 5 records
    vlt.CreateRecord({{"Name", std::string("Alexei Ivanov")}, {"City", std::string("Moscow")},
                      {"Gender", true}, {"Birth Year", std::uint16_t(1988)}});

    vlt.CreateRecord({{"Name", std::string("Maria Rodriguez")}, {"City", std::string("Madrid")},
                      {"Gender", false}, {"Birth Year", std::uint16_t(1990)}});

    vlt.CreateRecord({{"Name", std::string("Miguel Silva")}, {"City", std::string("Sao Paulo")},
                      {"Gender", true}, {"Birth Year", std::uint16_t(1994)}});

    vlt.CreateRecord({{"Name", std::string("Takashi Yamamoto")}, {"City", std::string("Tokyo")},
                      {"Gender", true}, {"Birth Year", std::uint16_t(1985)}});

    vlt.CreateRecord({{"Name", std::string("Amelia Thompson")}, {"City", std::string("Sydney")},
                      {"Gender", false}, {"Birth Year", std::uint16_t(1977)}});
    
    // Print vault
    std::cout << "Initial vault:" << std::endl;
    vlt.Print();


    // Get a record with the name Miguel Silva
    mvlt::VaultRecordRef vrr;
    vlt.GetRecord<std::string>("Name", "Miguel Silva", vrr);

    // Print record
    std::cout << "\n" << "Record:" << std::endl;
    vrr.PrintRecord();


    // Change city to Dubai
    vrr.SetData({{"City", std::string("Dubai")}});

    // Print updated vault
    std::cout << "\n" << "Updated vault:" << std::endl;
    vlt.Print();


    // Delete record
    vlt.EraseRecord(vrr);

    // Print updated vault
    std::cout << "\n" << "Vault after deletion:" << std::endl;
    vlt.Print();
}
