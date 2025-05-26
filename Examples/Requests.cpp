/*
    This file provides an example of basic query handling. 
    It will contain simple and complex requests.
*/

#include <MVault.h>

int main()
{
    // Create vault to store data
    mvlt::Vault vlt;

    // Add keys to vault
    vlt.AddKey("A", 0);
    vlt.AddKey("B", 0);

    // Fill vault with data
    for (int i = 0; i < 10; ++i)
        vlt.CreateRecord({{"A", i}, {"B", i}});

    // Print vault
    std::cout << "Initial vault:" << std::endl;
    vlt.Print();


    // Simple requests all records with A > 7
    mvlt::VaultRecordSet vrs;
    vlt.RequestGreater("A", 7, vrs);
    
    // Print set with A > 7 records
    std::cout << "\n" << "A > 7:" << std::endl;
    vrs.Print();


    // Complex request
    vlt.Request(mvlt::Less("A", 3) || mvlt::Greater("B", 7), vrs);

    // Print set with A < 3 or B > 7 records
    std::cout << "\n" << "A < 7 or B > 7:" << std::endl;
    vrs.Print();


    // Request with predicat. A > 4 and B < 7
    vlt.Request(mvlt::Greater("A", 4, [](const mvlt::VaultRecordRef& vrr) -> bool
    {
        int B = 0;
        vrr.GetData("B", B);
        if (B < 7)
            return true;
        else
            return false;
    }), vrs);

    // Print set with A > 4 and B < 7 records
    std::cout << "\n" << "A > 4 and B < 7:" << std::endl;
    vrs.Print();
}
