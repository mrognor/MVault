#include "VaultRequest.h"

#include "VaultRequest.hpp"

namespace mvlt
{
    void VaultRequest::Request(Vault* vlt, VaultRecordSet& vaultRecordSet) const
    {
        RequestFunc(vlt, vaultRecordSet);
    }
}