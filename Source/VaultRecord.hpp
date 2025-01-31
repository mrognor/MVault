#include "VaultRecord.h"

#include "Vault.h"
#include "VaultRecordSet.h"

namespace mvlt 
{
    template<class T>
    void VaultRecord::UpdateDependentSets(const std::string& key, const T& data) noexcept
    {
        DBG_LOG_ENTER();

        VaultRecordMutex.lock();

        for (VaultRecordSet* vaultRecordSet : dependentVaultRecordSets)
            vaultRecordSet->SetDataToRecord(this, key, data);

        VaultRecordMutex.unlock();
    }
}
