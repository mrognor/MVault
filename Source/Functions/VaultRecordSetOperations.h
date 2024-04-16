#pragma once

#include "../VaultRecordSet.h"

namespace mvlt 
{
    void Union(const VaultRecordSet& a, const VaultRecordSet& b, VaultRecordSet& res) noexcept;

    void Intersection(const VaultRecordSet& a, const VaultRecordSet& b, VaultRecordSet& res) noexcept;
}