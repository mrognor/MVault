#pragma once

#include "../../Source/MVault.h"

#include "TestLog.h"
#include "TestSuite.h"
#include "TestAssert.h"

#define COMPARE_VAULT(vlt, ...) TEST_ASSERT(vlt.ToJson(false, 2, false, "", true) == MTestsToJson(__VA_ARGS__))

struct TypeWrapper
{
    std::string Str;

    template<class T>
    TypeWrapper(const T& t)
    {
        Str = mvlt::ToJson(t);
    }
};

std::string MTestsToJson(const std::vector<std::vector<std::pair<std::string, TypeWrapper>>>& records);