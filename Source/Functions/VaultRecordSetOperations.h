#pragma once

#include "../VaultRecordSet.h"

namespace mvlt 
{
    void Union(const VaultRecordSet& a, const VaultRecordSet& b, VaultRecordSet& res);

    void Intersection(const VaultRecordSet& a, const VaultRecordSet& b, VaultRecordSet& res);
}