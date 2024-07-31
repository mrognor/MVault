#include "VaultRecordSetOperations.h"

namespace mvlt
{
    void Union(const VaultRecordSet& a, const VaultRecordSet& b, VaultRecordSet& res) noexcept
    {
        res = a;
        res.Join(b);
    }

    void Intersection(const VaultRecordSet& a, const VaultRecordSet& b, VaultRecordSet& res) noexcept
    {
        if (a.IsParentVaultValid)
        {
            a.ParentVault->RecursiveReadWriteMtx.ReadLock();

            res.ParentVault = a.ParentVault;
            res.IsParentVaultValid = true;
            
            for (auto& keyCopierIt : a.VaultKeyCopiers)
                keyCopierIt.second(res);

            a.ParentVault->RecordSetsSet.emplace(&res);

            // Pick less set to iterate 
            if (a.RecordsSet.size() < b.RecordsSet.size())
            {
                // Iterate over all records in a
                for (VaultRecord* record : a.RecordsSet)
                {
                    // if record in b then add it to the res
                    if (b.RecordsSet.find(record) != b.RecordsSet.end())
                    {
                        res.RecordsSet.emplace(record);
                    
                        for (auto& adder : res.VaultRecordAdders)
                            adder.second(record);
                    }
                }
            }
            else 
            {
                // Iterate over all records in b
                for (VaultRecord* record : b.RecordsSet)
                {
                    // if record in b then add it to the res
                    if (a.RecordsSet.find(record) != a.RecordsSet.end())
                    {
                        res.RecordsSet.emplace(record);
                    
                        for (auto& adder : res.VaultRecordAdders)
                            adder.second(record);
                    }
                }
            }

            a.ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
    }
}