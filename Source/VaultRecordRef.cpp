#include "VaultRecordRef.h"
#include "Vault.hpp"
#include "VaultRecordRef.hpp"

namespace mvlt 
{
    VaultRecordRef::VaultRecordRef() noexcept {}

    VaultRecordRef::VaultRecordRef(VaultRecord* vaultRecord, Vault* vlt) noexcept
    {
        SetRecord(vaultRecord, vlt);
    }

    VaultRecordRef::VaultRecordRef(const VaultRecordRef& other) noexcept
    {
        *this = other;
    }

    VaultRecordRef& VaultRecordRef::operator=(const VaultRecordRef& other) noexcept
    {
        if (&other != this)
        {
            VaultRecordRefMutex.lock();
            other.VaultRecordRefMutex.lock();

            if (other.VaultRecordPtr != nullptr)
            {
                other.VaultRecordPtr->VaultRecordMutex.lock();
                
                VaultRecord* oldVaultRecord = VaultRecordPtr;

                if (other.VaultRecordPtr->GetIsValid())
                {
                    if (other.VaultRecordPtr != nullptr) other.VaultRecordPtr->AddRef();

                    VaultRecordPtr = other.VaultRecordPtr;
                    Vlt = other.Vlt;
                }
                else 
                {
                    Vlt = nullptr;
                    VaultRecordPtr = nullptr;
                }
                
                other.VaultRecordPtr->VaultRecordMutex.unlock();

                if (oldVaultRecord != nullptr) oldVaultRecord->RemoveRef();
            }
            else 
            {
                if (VaultRecordPtr != nullptr) VaultRecordPtr->RemoveRef();
                Vlt = nullptr;
                VaultRecordPtr = nullptr;
            }

            other.VaultRecordRefMutex.unlock();
            VaultRecordRefMutex.unlock();
        }
        
        return *this;
    }

    bool VaultRecordRef::operator==(const VaultRecordRef& other) const noexcept
    {
        bool res;

        VaultRecordRefMutex.lock();
        other.VaultRecordRefMutex.lock();

        if (IsValid())
        {
            res = (VaultRecordPtr == other.VaultRecordPtr);
        }
        else
        {
            if (other.IsValid()) res = false;
            else res = true;
        }

        other.VaultRecordRefMutex.unlock();
        VaultRecordRefMutex.unlock();

        return res;
    }

    void VaultRecordRef::SetRecord(VaultRecord* vaultRecord, Vault* vlt) noexcept
    {
        VaultRecordRefMutex.lock();
        vlt->RecursiveReadWriteMtx.ReadLock();

        if (vaultRecord != nullptr) vaultRecord->AddRef();
        if (VaultRecordPtr != nullptr) VaultRecordPtr->RemoveRef();

        VaultRecordPtr = vaultRecord;
        Vlt = vlt;

        vlt->RecursiveReadWriteMtx.ReadUnlock();
        VaultRecordRefMutex.unlock();
    }

    std::string VaultRecordRef::GetRecordUniqueId() const noexcept
    {
        std::stringstream ss;

        VaultRecordRefMutex.lock();

        if (IsValid())
            ss << VaultRecordPtr;
        else
            ss << "null";

        VaultRecordRefMutex.unlock();
        
        return ss.str();
    }

    VaultOperationResult VaultRecordRef::SetData(const std::vector<std::pair<std::string, VaultParamInput>>& params) noexcept
    {
        VaultOperationResult res;
        VaultRecordRefMutex.lock();

        // Copy data from function parametrs
        for (const auto& paramsIt : params)
        {
            res = paramsIt.second.SetDataToRef(paramsIt.first, *this);
            if (!res.IsOperationSuccess) break;
        }

        VaultRecordRefMutex.unlock();

        return res;
    }
                
    VaultOperationResult VaultRecordRef::GetDataAsString(const std::string& key, std::string& str) const noexcept
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(std::string);

        VaultRecordRefMutex.lock();
        
        if (VaultRecordPtr == nullptr)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            VaultRecordRefMutex.unlock();
            return res;
        }

        // VaultRecordPtr lock VaultRecordRefMutex to lock VaultRecord::Invalidate in Vault destructor.
        // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
        VaultRecordPtr->VaultRecordMutex.lock();

        // Check if Vault still accessable
        if (!VaultRecordPtr->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            VaultRecordPtr->VaultRecordMutex.unlock();
            VaultRecordRefMutex.unlock();
            return res;
        }

        Vlt->RecursiveReadWriteMtx.ReadLock();

        // If key not exist
        if(!Vlt->GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;

            Vlt->RecursiveReadWriteMtx.ReadUnlock();
            VaultRecordPtr->VaultRecordMutex.unlock();
            VaultRecordRefMutex.unlock();
            return res;
        }

        VaultRecordPtr->GetDataAsString(key, str);
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        VaultRecordPtr->VaultRecordMutex.unlock();

        VaultRecordRefMutex.unlock();

        return res;
    }

    bool VaultRecordRef::IsValid() const noexcept
    {
        bool res = false;
        VaultRecordRefMutex.lock();
        if (VaultRecordPtr != nullptr) res = VaultRecordPtr->GetIsValid();
        VaultRecordRefMutex.unlock();
        return res;
    }

    bool VaultRecordRef::IsKeyExist(const std::string& key) const noexcept
    {
        bool res = false;
        VaultRecordRefMutex.lock();
        if (Vlt != nullptr) res = Vlt->IsKeyExist(key);
        VaultRecordRefMutex.unlock();
        return res;
    }

    std::vector<std::string> VaultRecordRef::GetKeys() const noexcept
    {
        std::vector<std::string> res;

        VaultRecordRefMutex.lock();

        if (VaultRecordPtr != nullptr)
        {
            // VaultRecordPtr lock VaultRecordRefMutex to lock VaultRecord::Invalidate in Vault destructor.
            // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
            VaultRecordPtr->VaultRecordMutex.lock();

            // Check if Vault still accessable
            if (VaultRecordPtr->GetIsValid())
            {
                Vlt->RecursiveReadWriteMtx.ReadLock();

                for (const auto& vaultMapStructureIt : Vlt->VaultMapStructure) res.emplace_back(vaultMapStructureIt.first);

                Vlt->RecursiveReadWriteMtx.ReadUnlock();
            }

            VaultRecordPtr->VaultRecordMutex.unlock();
        }

        VaultRecordRefMutex.unlock();

        return res;
    }

    void VaultRecordRef::PrintRecord() const noexcept
    {
        VaultRecordRefMutex.lock();

        if (VaultRecordPtr != nullptr)
        {
            // VaultRecordPtr lock VaultRecordRefMutex to lock VaultRecord::Invalidate in Vault destructor.
            // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
            VaultRecordPtr->VaultRecordMutex.lock();

            // Check if Vault still accessable
            if (VaultRecordPtr->GetIsValid())
            {
                Vlt->RecursiveReadWriteMtx.ReadLock();

                std::cout << "Vault record " << VaultRecordPtr << ":" << std::endl;

                for (const auto& keyPair : Vlt->VaultHashMapStructure)
                {
                    DataSaver dataSaver;
                    VaultRecordPtr->GetDataSaver(keyPair.first, dataSaver);
                
                    std::cout << "\t" << keyPair.first << " = " << dataSaver.Str() << std::endl;
                }

                Vlt->RecursiveReadWriteMtx.ReadUnlock();
            }
            else std::cout << "VaultRecordRef not valid!" << std::endl;

            VaultRecordPtr->VaultRecordMutex.unlock();
        }
        else std::cout << "VaultRecordRef not valid!" << std::endl;
        
        VaultRecordRefMutex.unlock();
    }

    void VaultRecordRef::Reset() noexcept
    {
        VaultRecordRefMutex.lock();

        if (VaultRecordPtr != nullptr)
            VaultRecordPtr->RemoveRef();
        
        VaultRecordPtr = nullptr;
        Vlt = nullptr;

        VaultRecordRefMutex.unlock();
    }

    void VaultRecordRef::ReadLock() const noexcept
    {
        VaultRecordRefMutex.lock();

        if (VaultRecordPtr != nullptr)
        {
            // VaultRecordPtr lock VaultRecordRefMutex to lock VaultRecord::Invalidate in Vault destructor.
            // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
            VaultRecordPtr->VaultRecordMutex.lock();

            Vlt->RecursiveReadWriteMtx.ReadLock();
        }

        VaultRecordRefMutex.unlock();
    }

    void VaultRecordRef::ReadUnlock() const noexcept
    {
        VaultRecordRefMutex.lock();

        if (VaultRecordPtr != nullptr)
        {
            // VaultRecordPtr lock VaultRecordRefMutex to lock VaultRecord::Invalidate in Vault destructor.
            // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
            VaultRecordPtr->VaultRecordMutex.lock();

            Vlt->RecursiveReadWriteMtx.ReadUnlock();
        }

        VaultRecordRefMutex.unlock();
    }
        
    VaultRecordRef::~VaultRecordRef() noexcept
    {
        if (VaultRecordPtr != nullptr) VaultRecordPtr->RemoveRef();
    }
}