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
        
        /// Provide access to Vaults PrintAsTable method
        using Vault::PrintAsTable;

        /// Provide access to Vaults Size method
        using Vault::Size;

        /// \brief Default constructor
        VaultRequestResult();

        /// \brief Copy constructor
        VaultRequestResult(const VaultRequestResult& other);

        /// \brief Operator assignment
        VaultRequestResult& operator=(const VaultRequestResult& other);

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
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            VaultOperationResult res = Vault::GetRecord(key, keyValue, vaultRecordRef);
            vaultRecordRef.Vlt = ParentVault;
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        // template <class T>
        // VaultOperationResult GetRecords(const std::string& key, const T& keyValue, std::vector<VaultRecordRef>& recordsRefs, const std::size_t& amountOfRecords = -1) const;

        // /// \todo Handle errors
        // template <class T>
        // void RequestRecords(const std::string& key, const T& keyValue, VaultRequestResult& vaultRequestResult);

        void Clear();

        // void RemoveRecordRef();

        // void RemoveRecordRefs();

        // void GetSortedRecords();

        // void SortBy();

        /// \brief Destructor
        ~VaultRequestResult();
    };
}