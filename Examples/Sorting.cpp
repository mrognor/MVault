/*
    This file demonstrates how to work with sorted data.
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

    // Iteration using sort
    vlt.SortBy("A", [&](const mvlt::VaultRecordRef& vrr)
    {
        vrr.PrintRecord();

        int B = 0;
        vrr.GetData("B", B);
        if (B > 5)
            return false;
        else
            return true;
    }, true);
}