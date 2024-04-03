#pragma once

#include "VaultRecordSet.h"

namespace mvlt
{
    template <class T>
    VaultOperationResult VaultRecordSet::RequestRecords(const RequestType& requestType, const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t amountOfRecords) const
    {
        VaultOperationResult res;
        RecursiveReadWriteMtx.ReadLock();

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            // \todo Replace to reset to clear keys 
            vaultRecordSet.Clear();
            res = Vault::RequestRecords(requestType, key, keyValue, vaultRecordSet, amountOfRecords);
            vaultRecordSet.ParentVault = ParentVault;

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(keyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        RecursiveReadWriteMtx.ReadUnlock();
        
        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordSet::GetKeyValue(const std::string& key, T& defaultKeyValue) const
    {
        VaultOperationResult res;
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            res = Vault::GetKeyValue(key, defaultKeyValue);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(defaultKeyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }
        RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordSet::GetRecord(const std::string& key, const T& keyValue, VaultRecordRef& vaultRecordRef) const
    {
        VaultOperationResult res;
        RecursiveReadWriteMtx.ReadLock();

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            res = Vault::GetRecord(key, keyValue, vaultRecordRef);
            vaultRecordRef.Vlt = ParentVault;
            
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(keyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        RecursiveReadWriteMtx.ReadUnlock();
        
        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordSet::GetRecords(const std::string& key, const T& keyValue, std::vector<VaultRecordRef>& recordsRefs, const std::size_t& amountOfRecords) const
    {
        VaultOperationResult res;
        RecursiveReadWriteMtx.ReadLock();

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            res = Vault::GetRecords(key, keyValue, recordsRefs);
            for (VaultRecordRef& ref: recordsRefs)
                ref.Vlt = ParentVault;
            
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(keyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        RecursiveReadWriteMtx.ReadUnlock();
        
        return res;
    }
    
    template <class T>
    VaultOperationResult VaultRecordSet::RequestEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords) const
    {
        return RequestRecords(RequestType::Equal, key, keyValue, vaultRecordSet, amountOfRecords);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestGreater(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords) const
    {
        return RequestRecords(RequestType::Greater, key, keyValue, vaultRecordSet, amountOfRecords);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestGreaterOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords) const
    {
        return RequestRecords(RequestType::GreaterOrEqual, key, keyValue, vaultRecordSet, amountOfRecords);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestLess(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords) const
    {
        return RequestRecords(RequestType::Less, key, keyValue, vaultRecordSet, amountOfRecords);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestLessOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords) const
    {
        return RequestRecords(RequestType::LessOrEqual, key, keyValue, vaultRecordSet, amountOfRecords);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RemoveRecord(const std::string& key, const T& keyValue)
    {
        VaultOperationResult res;
        RecursiveReadWriteMtx.ReadLock();

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            res = Vault::RemoveRecord(key, keyValue, false);

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(keyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        RecursiveReadWriteMtx.ReadUnlock();
        
        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RemoveRecords(const std::string& key, const T& keyValue, const std::size_t& amountOfRecords)
    {
        VaultOperationResult res;
        RecursiveReadWriteMtx.ReadLock();

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            res = Vault::RemoveRecords(key, keyValue, amountOfRecords, false);

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(keyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        RecursiveReadWriteMtx.ReadUnlock();
        
        return res;
    }

    template<class F>
    void VaultRecordSet::SortBy(const std::string& key, const F&& func, const bool& isReverse, const std::size_t& amountOfRecords) const
    {
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            Vault::SortBy(key, func, isReverse, amountOfRecords);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        RecursiveReadWriteMtx.ReadUnlock();
    }
}