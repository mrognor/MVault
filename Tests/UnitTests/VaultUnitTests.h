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

/// \brief Vault copy constructor test
TEST_SUITE(CopyConstructor,

    /// \brief Empty vault copy constructor test
    TEST_CASE(CopyEmptyVault)

    /// \brief Filled vault copy constructor test
    TEST_CASE(CopyFilledVault)
)

/// \brief Vault assignment operator test
TEST_SUITE(AssignmentOperator,

    /// \brief Empty vault assign test
    TEST_CASE(AssignEmptyVault)

    /// \brief Assign filled vault to empty
    TEST_CASE(AssignFilledToEmpty)

    /// \brief Assign empty vault to filled
    /// This test required to check vault clearing on assigning
    TEST_CASE(AssignEmptyToFilled)
)

/// \brief Vault move constructor test
TEST_SUITE(MoveConstructor,

    /// \brief Empty vault move test
    TEST_CASE(MoveEmptyConstructor)

    /// \brief Move filled vault to empty
    TEST_CASE(MoveFilledConstructor)
)

/// \brief Vault move assignment operator test
TEST_SUITE(MoveAssignmentOperator,

    /// \brief Empty vault assign test
    TEST_CASE(AssignEmptyVault)

    /// \brief Assign filled vault to empty
    TEST_CASE(AssignFilledToEmpty)

    /// \brief Assign empty vault to filled
    /// This test required to check vault clearing on assigning
    TEST_CASE(AssignEmptyToFilled)
)

void VaultUnitTests();