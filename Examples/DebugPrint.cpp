#include "../Source/MVault.h"


void five()
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
}

void four()
{
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
