#pragma once

#include "../../Source/MVault.h"

#include "TestLog.h"
#include "TestSuite.h"
#include "TestAssert.h"

struct TypeWrapper
{
    std::string Str;

    template<class T>
    TypeWrapper(const T& t)
    {
        Str = mvlt::ToString(t);
    }
};