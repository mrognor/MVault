#pragma once

#include "../../Source/MVault.h"

#include "TestLog.h"
#include "TestSuite.h"
#include "TestAssert.h"

#define COMPARE_VAULT(vlt, ...) TEST_ASSERT(CompareVault(vlt, __VA_ARGS__))

struct TypeWrapper
{
    std::string Str;

    template<class T>
    TypeWrapper(const T& t)
    {
        Str = mvlt::ToString(t);
    }
};

bool CompareVault(const mvlt::Vault& vlt, const std::vector<std::vector<std::pair<std::string, TypeWrapper>>>& records);