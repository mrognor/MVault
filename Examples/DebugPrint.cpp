#include "../Source/MVault.h"


void five()
{
    mvlt::Vault vlt;

    // int type
    vlt.AddKey("Int key", 0);
    vlt.CreateRecord({{"Int key", 1}});

    mvlt::VaultRecordRef vrf;
    vlt.GetRecord("Int key", 1, vrf);

    std::size_t a = 2;
    vrf.GetData("Int key", a);

    // 2
    std::cout << a << std::endl;
}

void four()
{
    mvlt::Vault vlt;

    // size_t type
    vlt.AddKey<std::size_t>("Size_t key", 0);

    // int type
    vlt.CreateRecord({{"Size_t key", 1}});

    // 2
    std::cout << vlt.Size() << std::endl;

    five();
}

void three()
{
    four();
}

void two()
{
    three();
}

void one()
{
    two();
}

int main()
{
    one();
}
