#include "VaultUnitTests.h"

TEST_BODY(DefaultConstructor, Default,

    Vault vlt;

    TEST_ASSERT(vlt.Size() == 0, "Failed to create Vault");
)

void VaultUnitTests()
{
    DefaultConstructor::Default();
}