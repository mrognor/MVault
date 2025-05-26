/*
    This file demonstrates error handling.
*/

#include <MVault.h>

int main()
{
    mvlt::Vault vlt;
    mvlt::VaultOperationResult vor;

    vlt.AddKey("A", 0);

    vor = vlt.CreateRecord({{"B", 0}});

    vor.Print();

    vor = vlt.CreateRecord({{"A", 0.0}});

    vor.Print();
}