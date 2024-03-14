#pragma once

#include "Vault.h"

namespace mvlt
{
    /** 
        \brief A class for storing query results
    
        The class inherits from Vault, but does not provide functionality for working with keys and creating new records.
        No new records will be created during the request, and this class will only store pointers to records in the original Vault.
    */
    class VaultRequestResult : protected Vault
    {
    private:
        // Pointer to parent Vault
        Vault* ParentVault = nullptr;

    public:

        /// Make Vault class frien
        friend Vault;

        /// Provide access to Vaults PrintVault method
        using Vault::PrintVault;
        
        /**
            \brief The method for getting a reference to the data inside Vault

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordRef, where information about the requested record will be recorded.
            If the key is not found, or the saved type does not match the type T, the parameter will not change.
            If the key was found and the types matched, but the value was not found, 
            a nullptr record will be saved to the ref and the vaultRecordRef.isValid() method returns false.

            The method will return Ref to the original Vault, but if the original Vault and Vault Request are changed, the Result will be changed

            \return VaultOperationResult object with GetRecord result
        */
        /// \todo Потокобезопасноть
        template <class T>
        VaultOperationResult GetRecord(const std::string& key, const T& keyValue, VaultRecordRef& vaultRecordRef) const
        {
            VaultOperationResult res = Vault::GetRecord(key, keyValue, vaultRecordRef);
            vaultRecordRef.Vlt = ParentVault;
            return res;
        }

        /// \brief Destructor
        ~VaultRequestResult()
        {
            RecursiveReadWriteMtx.WriteLock();
            ParentVault->RecursiveReadWriteMtx.WriteLock();

            // No need to delete each record because it is RequestResult and records will be delete in original vault
            RecordsSet.clear();

            // Remove this from records
            for (VaultRecord* record : RecordsSet)
            {
                record->Mtx->lock();
                record->dependentVaultRequestResults.erase(this);
                record->Mtx->unlock();
            }

            ParentVault->RecursiveReadWriteMtx.WriteUnlock();
            RecursiveReadWriteMtx.WriteUnlock();
        }
    };
}