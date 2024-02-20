#include "../Source/Vault.h"


int main()
{
    mvlt::Vault vlt;

    vlt.SetKey("Key1", -1);

    std::cout << "Wrong type check: " << std::endl;
    std::cout << "\tGetRecord" << std::endl;
    std::string key1Wrong;
    vlt.GetRecord("Key1", key1Wrong);


    std::cout << "\tCreateRecord" << std::endl;
    vlt.CreateRecord({ {"Key1", std::string("Z")} });
}