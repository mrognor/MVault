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
            Mtx.lock();
            other.Mtx.lock();

            if (other.DataRecord != nullptr)
            {
                other.DataRecord->Mtx.lock();
                
                VaultRecord* oldVaultRecord = DataRecord;

                if (other.DataRecord->GetIsValid())
                {
                    if (other.DataRecord != nullptr) other.DataRecord->AddRef();

                    DataRecord = other.DataRecord;
                    Vlt = other.Vlt;
                }
                else 
                {
                    Vlt = nullptr;
                    DataRecord = nullptr;
                }
                
                other.DataRecord->Mtx.unlock();

                if (oldVaultRecord != nullptr) oldVaultRecord->RemoveRef();
            }
            else 
            {
                if (DataRecord != nullptr) DataRecord->RemoveRef();
                Vlt = nullptr;
                DataRecord = nullptr;
            }

            other.Mtx.unlock();
            Mtx.unlock();
        }
        
        return *this;
    }

    bool VaultRecordRef::operator==(const VaultRecordRef& other) const noexcept
    {
        bool res;

        Mtx.lock();
        other.Mtx.lock();

        if (IsValid())
        {
            res = (DataRecord == other.DataRecord);
        }
        else
        {
            if (other.IsValid()) res = false;
            else res = true;
        }

        other.Mtx.unlock();
        Mtx.unlock();

        return res;
    }

    void VaultRecordRef::SetRecord(VaultRecord* vaultRecord, Vault* vlt) noexcept
    {
        Mtx.lock();
        vlt->RecursiveReadWriteMtx.ReadLock();

        if (vaultRecord != nullptr) vaultRecord->AddRef();
        if (DataRecord != nullptr) DataRecord->RemoveRef();

        DataRecord = vaultRecord;
        Vlt = vlt;

        vlt->RecursiveReadWriteMtx.ReadUnlock();
        Mtx.unlock();
    }

    std::string VaultRecordRef::GetRecordUniqueId() const noexcept
    {
        std::stringstream ss;

        Mtx.lock();

        if (IsValid())
            ss << DataRecord;
        else
            ss << "null";

        Mtx.unlock();
        
        return ss.str();
    }

    VaultOperationResult VaultRecordRef::SetData(const std::vector<std::pair<std::string, VaultParamInput>>& params) noexcept
    {
        VaultOperationResult res;
        Mtx.lock();

        // Copy data from function parametrs
        for (const auto& paramsIt : params)
        {
            res = paramsIt.second.SetDataToRef(paramsIt.first, *this);
            if (!res.IsOperationSuccess) break;
        }

        Mtx.unlock();

        return res;
    }
                
    VaultOperationResult VaultRecordRef::GetDataAsString(const std::string& key, std::string& str) const noexcept
    {
        // Fill res info known at start
        VaultOperationResult res;
        res.Key = key;
        res.RequestedType = typeid(std::string);

        Mtx.lock();
        
        if (DataRecord == nullptr)
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            Mtx.unlock();
            return res;
        }

        // DataRecord lock Mtx to lock VaultRecord::Invalidate in Vault destructor.
        // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
        DataRecord->Mtx.lock();

        // Check if Vault still accessable
        if (!DataRecord->GetIsValid())
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::DataRecordNotValid;

            DataRecord->Mtx.unlock();
            Mtx.unlock();
            return res;
        }

        Vlt->RecursiveReadWriteMtx.ReadLock();

        // If key not exist
        if(!Vlt->GetKeyType(key, res.SavedType))
        {
            res.IsOperationSuccess = false;
            res.ResultCode = VaultOperationResultCode::WrongKey;

            Vlt->RecursiveReadWriteMtx.ReadUnlock();
            DataRecord->Mtx.unlock();
            Mtx.unlock();
            return res;
        }

        DataRecord->GetDataAsString(key, str);
        res.IsOperationSuccess = true;
        res.ResultCode = VaultOperationResultCode::Success;

        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        DataRecord->Mtx.unlock();

        Mtx.unlock();

        return res;
    }

    bool VaultRecordRef::IsValid() const noexcept
    {
        bool res = false;
        Mtx.lock();
        if (DataRecord != nullptr) res = DataRecord->GetIsValid();
        Mtx.unlock();
        return res;
    }

    bool VaultRecordRef::IsKeyExist(const std::string& key) const noexcept
    {
        bool res = false;
        Mtx.lock();
        if (Vlt != nullptr) res = Vlt->IsKeyExist(key);
        Mtx.unlock();
        return res;
    }

    std::vector<std::string> VaultRecordRef::GetKeys() const noexcept
    {
        std::vector<std::string> res;

        Mtx.lock();

        if (DataRecord != nullptr)
        {
            // DataRecord lock Mtx to lock VaultRecord::Invalidate in Vault destructor.
            // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
            DataRecord->Mtx.lock();

            // Check if Vault still accessable
            if (DataRecord->GetIsValid())
            {
                Vlt->RecursiveReadWriteMtx.ReadLock();

                for (const auto& vaultMapStructureIt : Vlt->VaultMapStructure) res.emplace_back(vaultMapStructureIt.first);

                Vlt->RecursiveReadWriteMtx.ReadUnlock();
            }

            DataRecord->Mtx.unlock();
        }

        Mtx.unlock();

        return res;
    }

    void VaultRecordRef::PrintRecord() const noexcept
    {
        Mtx.lock();

        if (DataRecord != nullptr)
        {
            // DataRecord lock Mtx to lock VaultRecord::Invalidate in Vault destructor.
            // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
            DataRecord->Mtx.lock();

            // Check if Vault still accessable
            if (DataRecord->GetIsValid())
            {
                Vlt->RecursiveReadWriteMtx.ReadLock();

                std::cout << "Vault record " << DataRecord << ":" << std::endl;

                for (const auto& keyPair : Vlt->VaultHashMapStructure)
                {
                    DataSaver dataSaver;
                    DataRecord->GetDataSaver(keyPair.first, dataSaver);
                
                    std::cout << "\t" << keyPair.first << " = " << dataSaver.Str() << std::endl;
                }

                Vlt->RecursiveReadWriteMtx.ReadUnlock();
            }
            else std::cout << "VaultRecordRef not valid!" << std::endl;

            DataRecord->Mtx.unlock();
        }
        else std::cout << "VaultRecordRef not valid!" << std::endl;
        
        Mtx.unlock();
    }

    void VaultRecordRef::Reset() noexcept
    {
        Mtx.lock();

        if (DataRecord != nullptr)
            DataRecord->RemoveRef();
        
        DataRecord = nullptr;
        Vlt = nullptr;

        Mtx.unlock();
    }

    void VaultRecordRef::ReadLock() const noexcept
    {
        Mtx.lock();

        if (DataRecord != nullptr)
        {
            // DataRecord lock Mtx to lock VaultRecord::Invalidate in Vault destructor.
            // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
            DataRecord->Mtx.lock();

            Vlt->RecursiveReadWriteMtx.ReadLock();
        }

        Mtx.unlock();
    }

    void VaultRecordRef::ReadUnlock() const noexcept
    {
        Mtx.lock();

        if (DataRecord != nullptr)
        {
            // DataRecord lock Mtx to lock VaultRecord::Invalidate in Vault destructor.
            // It is necessary to prevent Vault::RecursiveReadWriteMtx from deletion 
            DataRecord->Mtx.lock();

            Vlt->RecursiveReadWriteMtx.ReadUnlock();
        }

        Mtx.unlock();
    }
        
    VaultRecordRef::~VaultRecordRef() noexcept
    {
        if (DataRecord != nullptr) DataRecord->RemoveRef();
    }
}