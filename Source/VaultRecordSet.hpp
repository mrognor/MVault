#pragma once

#include "VaultRecordSet.h"

namespace mvlt
{
    template <class T>
    VaultOperationResult VaultRecordSet::RequestRecords(const VaultRequestType& requestType, const std::string& key, const T& beginKeyValue,
            const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue, 
            const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        VaultOperationResult res;
        RecursiveReadWriteMtx.ReadLock();

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            // \todo Replace to reset to clear keys 
            vaultRecordSet.Clear();

            // Save vaultRecordSet
            ParentVault->RecordSetsSet.emplace(&vaultRecordSet);

            res = Vault::RequestRecords(requestType, key, beginKeyValue, endKeyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
            vaultRecordSet.ParentVault = ParentVault;

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(beginKeyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        RecursiveReadWriteMtx.ReadUnlock();
        
        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordSet::GetKeyValue(const std::string& key, T& defaultKeyValue) const noexcept
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
    VaultOperationResult VaultRecordSet::GetRecord(const std::string& key, const T& keyValue, VaultRecordRef& vaultRecordRef) const noexcept
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
    VaultOperationResult VaultRecordSet::GetRecords(const std::string& key, const T& keyValue, std::vector<VaultRecordRef>& recordsRefs, const std::size_t& amountOfRecords) const noexcept
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
    VaultOperationResult VaultRecordSet::RequestEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, 
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::Equal, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestGreater(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::Greater, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestGreaterOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, 
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::GreaterOrEqual, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestLess(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::Less, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestLessOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::LessOrEqual, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestInterval(const std::string& key, const T& beginKeyValue,
        const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue, 
        const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords,
        const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        return RequestRecords(VaultRequestType::Interval, key, beginKeyValue, 
            endKeyValue, vaultRecordSet, isIncludeBeginKeyValue, isIncludeEndKeyValue, 
            amountOfRecords, requestPredicat);
    }

    template <VaultRequestType Type>
    VaultOperationResult VaultRecordSet::Request(const VaultRequest<Type>&& request, VaultRecordSet& vaultRecordSet) const noexcept
    {
        VaultOperationResult res;
        RecursiveReadWriteMtx.ReadLock();

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            // \todo Replace to reset to clear keys 
            vaultRecordSet.Clear();

            // Save vaultRecordSet
            ParentVault->RecordSetsSet.emplace(&vaultRecordSet);

            res = Vault::Request(request, &vaultRecordSet);
            vaultRecordSet.ParentVault = ParentVault;

            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }
        else 
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        RecursiveReadWriteMtx.ReadUnlock();
        
        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RemoveRecord(const std::string& key, const T& keyValue) noexcept
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
    VaultOperationResult VaultRecordSet::RemoveRecords(const std::string& key, const T& keyValue, const std::size_t& amountOfRecords) noexcept
    {
        VaultOperationResult res;
        RecursiveReadWriteMtx.ReadLock();

        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();

            res = Vault::EraseRecords(key, keyValue, amountOfRecords);

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
    void VaultRecordSet::SortBy(const std::string& key, const F&& func, const bool& isReverse, const std::size_t& amountOfRecords) const noexcept
    {
        RecursiveReadWriteMtx.ReadLock();

        // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
        if (IsParentVaultValid)
        {
            ParentVault->RecursiveReadWriteMtx.ReadLock();
            Vault::SortBy(key, std::move(func), isReverse, amountOfRecords);
            ParentVault->RecursiveReadWriteMtx.ReadUnlock();
        }

        RecursiveReadWriteMtx.ReadUnlock();
    }
}