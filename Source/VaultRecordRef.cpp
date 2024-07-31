#include "VaultRecordRef.h"
#include "Vault.hpp"
#include "VaultRecordRef.hpp"

namespace mvlt 
{
    void VaultRecordRef::SetRecord(VaultRecord* vaultRecord, Vault* vlt) noexcept
    {
        vlt->RecursiveReadWriteMtx.ReadLock();

        if (vaultRecord != nullptr) vaultRecord->AddRef();
        if (VaultRecordPtr != nullptr) VaultRecordPtr->RemoveRef();

        VaultRecordPtr = vaultRecord;
        Vlt = vlt;

        vlt->RecursiveReadWriteMtx.ReadUnlock();
    }

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
            if (other.VaultRecordPtr != nullptr)
            {
                other.VaultRecordPtr->VaultRecordMutex.lock();
                
                VaultRecord* oldVaultRecord = VaultRecordPtr;

                if (other.VaultRecordPtr->GetIsValid())
                {
                    other.VaultRecordPtr->AddRef();

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
        }
        
        return *this;
    }

    bool VaultRecordRef::operator==(const VaultRecordRef& other) const noexcept
    {
        bool res;

        if (IsValid())
        {
            res = (VaultRecordPtr == other.VaultRecordPtr);
        }
        else
        {
            if (other.IsValid()) res = false;
            else res = true;
        }

        return res;
    }

    std::string VaultRecordRef::GetRecordUniqueId() const noexcept
    {
        std::stringstream ss;

        if (IsValid())
            ss << VaultRecordPtr;
        else
            ss << "null";

        return ss.str();
    }

    VaultOperationResult VaultRecordRef::SetData(const std::vector<std::pair<std::string, VaultParamInput>>& params) noexcept
    {
        VaultOperationResult res;

        // Copy data from function parametrs
        for (const auto& paramsIt : params)
        {
            res = paramsIt.second.SetDataToRef(paramsIt.first, *this);
            if (!res.IsOperationSuccess) break;
        }

        return res;
    }
                
    VaultOperationResult VaultRecordRef::GetDataAsString(const std::string& key, std::string& str) const noexcept
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(std::string);
        
        if (VaultRecordPtr == nullptr)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            return res;
        }

        VaultRecordPtr->VaultRecordMutex.lock();

        // Check if Vault still accessable
        if (!VaultRecordPtr->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            VaultRecordPtr->VaultRecordMutex.unlock();
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
            return res;
        }

        VaultRecordPtr->GetDataAsString(key, str);
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        VaultRecordPtr->VaultRecordMutex.unlock();

        return res;
    }

    bool VaultRecordRef::IsValid() const noexcept
    {
        bool res = false;

        if (VaultRecordPtr != nullptr) res = VaultRecordPtr->GetIsValid();

        return res;
    }

    bool VaultRecordRef::IsKeyExist(const std::string& key) const noexcept
    {
        bool res = false;

        if (Vlt != nullptr) res = Vlt->IsKeyExist(key);

        return res;
    }

    std::vector<std::string> VaultRecordRef::GetKeys() const noexcept
    {
        std::vector<std::string> res;

        if (VaultRecordPtr != nullptr)
        {
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

        return res;
    }

    void VaultRecordRef::PrintRecord() const noexcept
    {
        if (VaultRecordPtr != nullptr)
        {
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
    }

    void VaultRecordRef::Reset() noexcept
    {
        if (VaultRecordPtr != nullptr)
            VaultRecordPtr->RemoveRef();
        
        VaultRecordPtr = nullptr;
        Vlt = nullptr;
    }

    VaultRecordRef::~VaultRecordRef() noexcept
    {
        if (VaultRecordPtr != nullptr) VaultRecordPtr->RemoveRef();
    }
}