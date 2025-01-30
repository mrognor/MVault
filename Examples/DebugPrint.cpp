#include "../Source/MVault.h"


void five(mvlt::Vault& vlt)
{
    mvlt::VaultRecordRef vrf;
    vlt.GetRecord("A", 1, vrf);

    std::size_t a = 4;
    vrf.GetData("A", a);

    // 4
    std::cout << a << std::endl;
}

void four(mvlt::Vault& vlt)
{
    five(vlt);
}

void three(mvlt::Vault& vlt)
{
    four(vlt);
}

void two(mvlt::Vault& vlt)
{
    three(vlt);
}

void one(mvlt::Vault& vlt)
{
    two(vlt);
}

int main()
{
    mvlt::Vault vlt;

    // int type
    vlt.AddKey("A", 0);
    vlt.CreateRecord({{"A", 1}});

    mvlt::VaultRecordRef vrf;
    vlt.GetRecord("A", 1, vrf);

    std::size_t a = 2;
    vrf.GetData("A", a);

    // 2
    std::cout << a << std::endl;
    one(vlt);
}
