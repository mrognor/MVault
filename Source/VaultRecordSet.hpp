#pragma once

#include "VaultRecordSet.h"

namespace mvlt
{
    template <class T>
    VaultOperationResult VaultRecordSet::RequestRecords(const VaultRequestType& requestType, const std::string& key, const T& beginKeyValue,
            const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue, 
            const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;

        if (GetIsParentVaultValid())
        {
            if (&vaultRecordSet == this)
            {
                res.IsOperationSuccess = false;
                res.ResultCode = VaultOperationResultCode::SameVaultRecordSet;
            }
            else 
            {
                ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);

                // Remove old data from vaultRecordSet
                if (vaultRecordSet.ParentVault != ParentVault)
                {
                    vaultRecordSet.Reset();

                    // Set new parent vault to vaultRecordSet
                    vaultRecordSet.ParentVault = ParentVault;

                    // Copy keys from this to vaultRecordSet
                    for (auto& keyCopierIt : VaultKeyCopiers)
                        keyCopierIt.second(&vaultRecordSet);

                    // Set proper key order
                    vaultRecordSet.KeysOrder = KeysOrder;

                    // Set unique keys
                    vaultRecordSet.UniqueKeys = UniqueKeys;
                }
                else vaultRecordSet.Clear();

                // Save vaultRecordSet
                ParentVault->RecordSetsSet.emplace(&vaultRecordSet);

                res = Vault::RequestRecords(requestType, key, beginKeyValue, endKeyValue, vaultRecordSet, isIncludeBeginKeyValue, isIncludeEndKeyValue, amountOfRecords, requestPredicat);
                vaultRecordSet.ParentVault = ParentVault;
            }
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(beginKeyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordSet::GetKeyValue(const std::string& key, T& defaultKeyValue) const noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            res = Vault::GetKeyValue(key, defaultKeyValue);
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(defaultKeyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordSet::GetRecord(const std::string& key, const T& keyValue, VaultRecordRef& vaultRecordRef) const noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);

            res = Vault::GetRecord(key, keyValue, vaultRecordRef);
            vaultRecordRef.Vlt = ParentVault;
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(keyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }
        
        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordSet::GetRecords(const std::string& key, const T& keyValue, std::vector<VaultRecordRef>& recordsRefs, const std::size_t& amountOfRecords) const noexcept
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);

            res = Vault::GetRecords(key, keyValue, recordsRefs, amountOfRecords);
            for (VaultRecordRef& ref: recordsRefs)
                ref.Vlt = ParentVault;
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(keyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }
        
        return res;
    }
    
    template <class T>
    VaultOperationResult VaultRecordSet::RequestEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, 
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const
    {
        DBG_LOG_ENTER();

        return RequestRecords(VaultRequestType::Equal, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestGreater(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const
    {
        DBG_LOG_ENTER();

        return RequestRecords(VaultRequestType::Greater, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestGreaterOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, 
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const
    {
        DBG_LOG_ENTER();

        return RequestRecords(VaultRequestType::GreaterOrEqual, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestLess(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const
    {
        DBG_LOG_ENTER();

        return RequestRecords(VaultRequestType::Less, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestLessOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
        const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const
    {
        DBG_LOG_ENTER();

        return RequestRecords(VaultRequestType::LessOrEqual, key, keyValue, keyValue, vaultRecordSet, false, false, amountOfRecords, requestPredicat);
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RequestInterval(const std::string& key, const T& beginKeyValue,
        const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue, 
        const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords,
        const std::function<bool(const VaultRecordRef&)>& requestPredicat) const
    {
        DBG_LOG_ENTER();

        return RequestRecords(VaultRequestType::Interval, key, beginKeyValue, 
            endKeyValue, vaultRecordSet, isIncludeBeginKeyValue, isIncludeEndKeyValue, 
            amountOfRecords, requestPredicat);
    }

    template <VaultRequestType Type>
    VaultOperationResult VaultRecordSet::Request(VaultRequest<Type>&& request, VaultRecordSet& vaultRecordSet) const
    {
        DBG_LOG_ENTER();

        VaultOperationResult res;

        if (GetIsParentVaultValid())
        {
            if (&vaultRecordSet == this)
            {
                res.IsOperationSuccess = false;
                res.ResultCode = VaultOperationResultCode::SameVaultRecordSet;
            }
            else 
            {
                ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);

                // Remove old data from vaultRecordSet
                if (vaultRecordSet.ParentVault != ParentVault)
                {
                    vaultRecordSet.Reset();

                    // Set new parent vault to vaultRecordSet
                    vaultRecordSet.ParentVault = ParentVault;

                    // Copy keys from this to vaultRecordSet
                    for (auto& keyCopierIt : VaultKeyCopiers)
                        keyCopierIt.second(&vaultRecordSet);

                    // Set proper key order
                    vaultRecordSet.KeysOrder = KeysOrder;

                    // Set unique keys
                    vaultRecordSet.UniqueKeys = UniqueKeys;
                }
                else vaultRecordSet.Clear();

                // Save vaultRecordSet
                ParentVault->RecordSetsSet.emplace(&vaultRecordSet);

                res = Vault::Request(std::move(request), vaultRecordSet);
                vaultRecordSet.ParentVault = ParentVault;
            }
        }
        else 
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RemoveRecord(const std::string& key, const T& keyValue) noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            RemoveRecord<std::string>(\"Key\", \"Value\") or RemoveRecord(\"Key\", std::string(\"Value\"))");

        VaultOperationResult res;

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            res = Vault::EraseRecord(key, keyValue);
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(keyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }
        
        return res;
    }

    template <class T>
    VaultOperationResult VaultRecordSet::RemoveRecords(const std::string& key, const T& keyValue, const std::size_t& amountOfRecords) noexcept
    {
        DBG_LOG_ENTER();

        static_assert(!std::is_array<T>::value, "It is not possible to use a c array as a key value. \n\
            If you want to use a string as a key, you must specialize the function with a string. Like this: \n\
            RemoveRecords<std::string>(\"Key\", \"Value\") or RemoveRecords(\"Key\", std::string(\"Value\"))");

        VaultOperationResult res;

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            res = Vault::EraseRecords(key, keyValue, amountOfRecords);
        }
        else 
        {
            res.Key = key;
            res.RequestedType = typeid(keyValue);
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
        }

        return res;
    }

    template<class F>
    void VaultRecordSet::SortBy(const std::string& key, F&& func, const bool& isReverse, const std::size_t& amountOfRecords) const noexcept
    {
        DBG_LOG_ENTER();

        if (GetIsParentVaultValid())
        {
            ReadLock<RecursiveReadWriteMutex> readLock(ParentVault->RecursiveReadWriteMtx);
            Vault::SortBy(key, std::move(func), isReverse, amountOfRecords);
        }
    }
}