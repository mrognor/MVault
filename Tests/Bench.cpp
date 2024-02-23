#include "Tests.h"

void Bench()
{
    mvlt::Vault vlt;
    vlt.SetKey<std::size_t>("id", -1);

    for (std::size_t i = 0; i < 10000000; ++i)
        vlt.CreateRecord({ {"id", i} });
    
    mvlt::VaultRecordRef vrr;

    auto start = std::chrono::steady_clock::now();

    for (std::size_t i = 0; i < 15000; i += 15)
        vlt.GetRecord("id", i, vrr);

    auto end = std::chrono::steady_clock::now();

    std::cout << "Bench result: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << std::endl;
}