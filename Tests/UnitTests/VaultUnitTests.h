#pragma once

#include "../../Source/MVault.h"

#include "../MTests/TestLog.h"
#include "../MTests/TestSuite.h"
#include "../MTests/TestAssert.h"
#include "../MTests/CompareVault.h"

using namespace mvlt;

/// \brief Vault default constructor test
TEST_SUITE(DefaultConstructor,

    /// \brief Default constructor test
    TEST_CASE(Default)
)

void VaultUnitTests();