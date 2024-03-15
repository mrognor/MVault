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

            if (other.IsValid())
            {
                other.Vlt->RecursiveReadWriteMtx.ReadLock();

                if (DataRecord != nullptr) DataRecord->RemoveRef();
                other.DataRecord->AddRef();

                DataRecord = other.DataRecord;
                Vlt = other.Vlt;

                other.Vlt->RecursiveReadWriteMtx.ReadUnlock();
            }

            // \todo else
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

        if (IsValid())
        {
            Vlt->RecursiveReadWriteMtx.ReadLock();

            res = (DataRecord == other.DataRecord);
            
            Vlt->RecursiveReadWriteMtx.ReadUnlock();
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

    void VaultRecordRef::SetRecord(VaultRecord* vaultRecord, Vault* vlt)
    {
        Mtx.lock();
        vlt->RecursiveReadWriteMtx.ReadLock();

        if (DataRecord != nullptr) DataRecord->RemoveRef();
        if (vaultRecord != nullptr) vaultRecord->AddRef();
        
        DataRecord = vaultRecord;
        Vlt = vlt;

        vlt->RecursiveReadWriteMtx.ReadUnlock();
        Mtx.unlock();
    }

    std::string VaultRecordRef::GetRecordUniqueId() const
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
        bool res = false;

        Mtx.lock();
        
        if (IsValid())
        {
            Vlt->RecursiveReadWriteMtx.ReadLock();

            if (DataRecord->GetIsValid()) res = DataRecord->GetDataAsString(key, str);

            Vlt->RecursiveReadWriteMtx.ReadUnlock();
        }

        Mtx.unlock();

        return res;
    }

    bool VaultRecordRef::IsValid() const
    {
        bool res = false;
        Mtx.lock();
        if (DataRecord != nullptr) res = DataRecord->GetIsValid();
        Mtx.unlock();
        return res;
    }

    std::vector<std::string> VaultRecordRef::GetKeys() const
    {
        std::vector<std::string> res;

        Mtx.lock();

        if (IsValid())
        {
            Vlt->RecursiveReadWriteMtx.ReadLock();

            for (const auto& it : Vlt->VaultMapStructure) res.emplace_back(it.first);
        
            Vlt->RecursiveReadWriteMtx.ReadUnlock();
        }

        Mtx.unlock();

        return res;
    }

    void VaultRecordRef::PrintRecord() const
    {
        Mtx.lock();

        if (IsValid())
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

        Mtx.unlock();
    }

    void VaultRecordRef::Reset()
    {
        Mtx.lock();

        if (DataRecord != nullptr)
            DataRecord->RemoveRef();
        
        DataRecord = nullptr;
        Vlt = nullptr;

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