#pragma once

#include "TypeWrapper.h"

template <mvlt::VaultRequestType Type>
std::vector<std::vector<std::pair<std::string, TypeWrapper>>> CheckRequest(const mvlt::Vault& vlt, mvlt::VaultRecordSet& vrs, mvlt::VaultOperationResult& vor,
     const mvlt::VaultRequest<Type>& request, const std::vector<std::vector<std::pair<std::string, TypeWrapper>>>& records)
{
    vor = vlt.Request(request, vrs);
    return records;
}

#define COMPARE_CORRECT_REQUEST(...) \
records = CheckRequest(vlt, vrs, vor, __VA_ARGS__); \
TEST_ASSERT(vrs.Size() == records.size()); \
COMPARE_VAULT(vrs, records); \
COMPARE_OPERATION(vor, IsOperationSuccess == true, ResultCode == VaultOperationResultCode::Success);


template <mvlt::VaultRequestType Type>
std::vector<std::vector<std::pair<std::string, TypeWrapper>>> CheckRequest(const mvlt::VaultRecordSet& parentVrs, mvlt::VaultRecordSet& vrs, mvlt::VaultOperationResult& vor,
     const mvlt::VaultRequest<Type>& request, const std::vector<std::vector<std::pair<std::string, TypeWrapper>>>& records)
{
    vor = parentVrs.Request(request, vrs);
    return records;
}

#define COMPARE_CORRECT_REQUEST_SET(...) \
records = CheckRequest(parentVrs, vrs, vor, __VA_ARGS__); \
TEST_ASSERT(vrs.Size() == records.size()); \
COMPARE_VAULT(vrs, records); \
COMPARE_OPERATION(vor, IsOperationSuccess == true, ResultCode == VaultOperationResultCode::Success);
