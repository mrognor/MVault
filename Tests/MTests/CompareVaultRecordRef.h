#pragma once

#include "TypeWrapper.h"

#define COMPARE_REF(vrr, ...) TEST_ASSERT(CompareRef(vrr, __VA_ARGS__))

bool CompareRef(const mvlt::VaultRecordRef& vrr, const std::vector<std::pair<std::string, TypeWrapper>>& record);