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
            /// \todo Надо ли тут блокировать other
            other.Vlt->RecursiveReadWriteMtx.ReadLock();
            Mtx.lock();
            other.Mtx.lock();

            if (DataRecord != nullptr) DataRecord->RemoveRef();
            if (other.DataRecord != nullptr) other.DataRecord->AddRef();

            IsRefValid = other.IsRefValid; 
            DataRecord = other.DataRecord;
            Vlt = other.Vlt;

            other.Mtx.unlock();
            Mtx.unlock();
            other.Vlt->RecursiveReadWriteMtx.ReadUnlock();
        }
        
        return *this;
    }

    bool VaultRecordRef::operator==(const VaultRecordRef& other) const
    {
        bool res;
        /// \todo Надо ли тут блокировать other
        Vlt->RecursiveReadWriteMtx.ReadLock();
        Mtx.lock();
        other.Mtx.lock();

        res = (DataRecord == other.DataRecord);
        
        other.Mtx.unlock();
        Mtx.unlock();
        Vlt->RecursiveReadWriteMtx.ReadUnlock();
        return res;
    }

    void VaultRecordRef::SetRecord(VaultRecord* vaultRecord, Vault* vlt)
    {
        vlt->RecursiveReadWriteMtx.ReadLock();
        Mtx.lock();

        if (DataRecord != nullptr) DataRecord->RemoveRef();
        if (vaultRecord != nullptr) 
        {
            vaultRecord->AddRef();
            IsRefValid = true;
        }
        else IsRefValid = false;
        
        DataRecord = vaultRecord;
        Vlt = vlt;

        Mtx.unlock();
        vlt->RecursiveReadWriteMtx.ReadUnlock();
    }

    std::string VaultRecordRef::GetRecordUniqueId() const
    {
        std::stringstream ss;

        Vlt->RecursiveReadWriteMtx.ReadLock();
        Mtx.lock();

        if (IsValid())
            ss << DataRecord;
        else
            ss << "null";

        Mtx.unlock();
        Vlt->RecursiveReadWriteMtx.ReadUnlock();

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
        Vlt->RecursiveReadWriteMtx.ReadLock();
        Mtx.lock();

        if (DataRecord->GetIsValid()) res = DataRecord->GetDataAsString(key, str);

        Mtx.unlock();
        Vlt->RecursiveReadWriteMtx.ReadUnlock();
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

        Vlt->RecursiveReadWriteMtx.ReadLock();
        Mtx.lock();

        for (const auto& it : Vlt->VaultMapStructure) res.emplace_back(it.first);
        
        Mtx.unlock();
        Vlt->RecursiveReadWriteMtx.ReadUnlock();

        return res;
    }

    void VaultRecordRef::PrintRecord() const
    {
        Vlt->RecursiveReadWriteMtx.ReadLock();
        Mtx.lock();

        std::cout << "Vault record " << DataRecord << ":" << std::endl;

        for (const auto& keyPair : Vlt->VaultHashMapStructure)
        {
            DataSaver dataSaver;
            DataRecord->GetDataSaver(keyPair.first, dataSaver);
        
            std::cout << "\t" << keyPair.first << " = " << dataSaver.Str() << std::endl;
        }

        Mtx.unlock();
        Vlt->RecursiveReadWriteMtx.ReadUnlock();
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