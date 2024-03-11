#include "VaultRecordRef.h"
#include "Vault.hpp"
#include "VaultRecordRef.hpp"

namespace mvlt 
{
    VaultRecordRef::VaultRecordRef() {}

    VaultRecordRef::VaultRecordRef(VaultRecord* vaultRecord, Vault* vlt)
    {
        SetRecord(vaultRecord, vlt);
    }

    VaultRecordRef::VaultRecordRef(const VaultRecordRef& other)
    {
        *this = other;
    }

    VaultRecordRef& VaultRecordRef::operator=(const VaultRecordRef& other)
    {
        if (&other != this)
        {
            Mtx.lock();
            other.Mtx.lock();
            /// \todo Надо ли тут блокировать other
            other.Vlt->RecursiveReadWriteMtx.ReadLock();

            if (DataRecord != nullptr) DataRecord->RemoveRef();
            if (other.DataRecord != nullptr) other.DataRecord->AddRef();

            IsRefValid = other.IsRefValid; 
            DataRecord = other.DataRecord;
            Vlt = other.Vlt;

            other.Vlt->RecursiveReadWriteMtx.ReadUnlock();
            other.Mtx.unlock();
            Mtx.unlock();
        }
        
        return *this;
    }

    bool VaultRecordRef::operator==(const VaultRecordRef& other) const
    {
        bool res;

        Mtx.lock();
        other.Mtx.lock();
        /// \todo Надо ли тут блокировать other
        Vlt->RecursiveReadWriteMtx.ReadLock();

        res = (DataRecord == other.DataRecord);
        
        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        other.Mtx.unlock();
        Mtx.unlock();

        return res;
    }

    void VaultRecordRef::SetRecord(VaultRecord* vaultRecord, Vault* vlt)
    {
        Mtx.lock();
        vlt->RecursiveReadWriteMtx.ReadLock();

        if (DataRecord != nullptr) DataRecord->RemoveRef();
        if (vaultRecord != nullptr) 
        {
            vaultRecord->AddRef();
            IsRefValid = true;
        }
        else IsRefValid = false;
        
        DataRecord = vaultRecord;
        Vlt = vlt;

        vlt->RecursiveReadWriteMtx.ReadUnlock();
        Mtx.unlock();
    }

    std::string VaultRecordRef::GetRecordUniqueId() const
    {
        std::stringstream ss;

        Mtx.lock();
        Vlt->RecursiveReadWriteMtx.ReadLock();

        if (IsValid())
            ss << DataRecord;
        else
            ss << "null";

        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        Mtx.unlock();
        
        return ss.str();
    }

    void VaultRecordRef::SetData(const std::vector<std::pair<std::string, DataSaver>>& params)
    {
        Mtx.lock();
        /// \todo Это вообще не правильно
        // Copy data from function parametrs
        for (auto& it : params)
            if (DataRecord->IsData(it.first))
                DataRecord->SetDataFromDataSaver(it.first, it.second);
        Mtx.unlock();
    }

    bool VaultRecordRef::GetDataAsString(const std::string& key, std::string& str) const
    {
        bool res;

        Mtx.lock();
        Vlt->RecursiveReadWriteMtx.ReadLock();

        if (DataRecord->GetIsValid()) res = DataRecord->GetDataAsString(key, str);

        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        Mtx.unlock();

        return res;
    }

    bool VaultRecordRef::IsValid() const
    {
        bool res = false;
        Mtx.lock();
        if (DataRecord != nullptr && IsRefValid) res = DataRecord->GetIsValid();
        Mtx.unlock();
        return res;
    }

    std::vector<std::string> VaultRecordRef::GetKeys() const
    {
        std::vector<std::string> res;

        Mtx.lock();
        Vlt->RecursiveReadWriteMtx.ReadLock();

        for (const auto& it : Vlt->VaultMapStructure) res.emplace_back(it.first);
        
        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        Mtx.unlock();

        return res;
    }

    void VaultRecordRef::PrintRecord() const
    {
        Mtx.lock();
        Vlt->RecursiveReadWriteMtx.ReadLock();

        std::cout << "Vault record " << DataRecord << ":" << std::endl;

        for (const auto& keyPair : Vlt->VaultHashMapStructure)
        {
            DataSaver dataSaver;
            DataRecord->GetDataSaver(keyPair.first, dataSaver);
        
            std::cout << "\t" << keyPair.first << " = " << dataSaver.Str() << std::endl;
        }

        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        Mtx.unlock();
    }

    void VaultRecordRef::Unlink()
    {
        Mtx.lock();
        DataRecord = nullptr;
        Vlt = nullptr;
        IsRefValid = false;
        Mtx.unlock();
    }

    void VaultRecordRef::ReadLock() const
    {
        Vlt->RecursiveReadWriteMtx.ReadLock();
    }

    void VaultRecordRef::ReadUnlock() const
    {
        Vlt->RecursiveReadWriteMtx.ReadUnlock();
    }
        
    VaultRecordRef::~VaultRecordRef()
    {
        if (DataRecord != nullptr) DataRecord->RemoveRef();
    }
}