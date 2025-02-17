#include "VaultUnitTests.h"

TEST_BODY(DefaultConstructor, Default,
    Vault vlt;

    TEST_ASSERT(vlt.Size() == 0);
    TEST_ASSERT(vlt.GetKeys().size() == 0);
    TEST_ASSERT(vlt.ToJson() == "{}");

    COMPARE_VAULT(vlt, {});

    vlt.AddKey("A", 0);
    vlt.AddKey<std::string>("B", "");
    
    vlt.CreateRecord({ {"A", 1}, {"B", std::string("a")} });
    vlt.CreateRecord({ {"A", 2}, {"B", std::string("b")} });
    vlt.CreateRecord({ {"A", 3}, {"B", std::string("c")} });

    COMPARE_VAULT(vlt, {
        {{"A", 3}, {"B",  std::string("c")}},
        {{"A", 2}, {"B",  std::string("b")}},
        {{"A", 1}, {"B",  std::string("a")}},
    });
)

void VaultUnitTests()
{
    DefaultConstructor::Default();
}