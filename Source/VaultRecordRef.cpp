#include "VaultRecordRef.h"
#include "Vault.hpp"
#include "VaultRecordRef.hpp"

namespace mvlt 
{
    VaultRecordRef::VaultRecordRef() {}

    VaultRecordRef::VaultRecordRef(VaultRecord* vaultRecord, 
        VaultStructureHashMap* vaultStructureHashMap, 
        VaultStructureMap* vaultStructureMap,
        RecursiveReadWriteMutex* vaultRecursiveReadWriteMtx)
    {
        SetRecord(vaultRecord, vaultStructureHashMap, vaultStructureMap, vaultRecursiveReadWriteMtx);
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
            other.VaultRecucrsiveReadWriteMtx->ReadLock();
            if (DataRecord != nullptr) DataRecord->RemoveRef();
            if (other.DataRecord != nullptr) other.DataRecord->AddRef();
            DataRecord = other.DataRecord;
            VaultHashMapStructure = other.VaultHashMapStructure;
            VaultMapStructure = other.VaultMapStructure;
            VaultRecucrsiveReadWriteMtx = other.VaultRecucrsiveReadWriteMtx;
            other.VaultRecucrsiveReadWriteMtx->ReadUnlock();
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
        VaultRecucrsiveReadWriteMtx->ReadLock();
        res = (DataRecord == other.DataRecord);
        VaultRecucrsiveReadWriteMtx->ReadUnlock();
        other.Mtx.unlock();
        Mtx.unlock();
        return res;
    }

    void VaultRecordRef::SetRecord(VaultRecord* vaultRecord, 
        VaultStructureHashMap* vaultStructureHashMap, 
        VaultStructureMap* vaultStructureMap,
        RecursiveReadWriteMutex* vaultRecursiveReadWriteMtx)
    {
        Mtx.lock();
        vaultRecursiveReadWriteMtx->ReadLock();
        if (DataRecord != nullptr) DataRecord->RemoveRef();
        if (vaultRecord != nullptr) vaultRecord->AddRef();
        DataRecord = vaultRecord;
        VaultHashMapStructure = vaultStructureHashMap;
        VaultMapStructure = vaultStructureMap;
        VaultRecucrsiveReadWriteMtx = vaultRecursiveReadWriteMtx;
        vaultRecursiveReadWriteMtx->ReadUnlock();
        Mtx.unlock();
    }

    std::string VaultRecordRef::GetRecordUniqueId() const
    {
        std::stringstream ss;
        Mtx.lock();
        VaultRecucrsiveReadWriteMtx->ReadLock();
        if (IsValid())
            ss << DataRecord;
        else
            ss << "null";
        VaultRecucrsiveReadWriteMtx->ReadUnlock();
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
        VaultRecucrsiveReadWriteMtx->ReadLock();
        if (DataRecord->GetIsValid()) res = DataRecord->GetDataAsString(key, str);
        VaultRecucrsiveReadWriteMtx->ReadUnlock();
        Mtx.unlock();
        return res;
    }

    bool VaultRecordRef::IsValid() const
    {
        /// \todo А точно?
        // Thread saferty
        if (DataRecord != nullptr)
            return DataRecord->GetIsValid();
        else
            return false;
    }

    std::vector<std::string> VaultRecordRef::GetKeys() const
    {
        std::vector<std::string> res;

        Mtx.lock();
        VaultRecucrsiveReadWriteMtx->ReadLock();
        for (const auto& it : *VaultMapStructure) res.emplace_back(it.first);
        VaultRecucrsiveReadWriteMtx->ReadUnlock();
        Mtx.unlock();

        return res;
    }

    void VaultRecordRef::PrintRecord() const
    {
        Mtx.lock();
        VaultRecucrsiveReadWriteMtx->ReadLock();
        
        std::cout << "Vault record " << DataRecord << ":" << std::endl;

        for (const auto& keyPair : *VaultHashMapStructure)
        {
            DataSaver dataSaver;
            DataRecord->GetDataSaver(keyPair.first, dataSaver);
        
            std::cout << "\t" << keyPair.first << " = " << dataSaver.Str() << std::endl;
        }

        VaultRecucrsiveReadWriteMtx->ReadUnlock();
        Mtx.unlock();
    }

    void VaultRecordRef::Unlink()
    {
        Mtx.lock();
        DataRecord = nullptr;
        VaultHashMapStructure = nullptr;
        VaultMapStructure = nullptr;
        Mtx.unlock();
    }

    void VaultRecordRef::ReadLock() const
    {
        VaultRecucrsiveReadWriteMtx->ReadLock();
    }

    void VaultRecordRef::ReadUnlock() const
    {
        VaultRecucrsiveReadWriteMtx->ReadUnlock();
    }
        
    VaultRecordRef::~VaultRecordRef()
    {
        if (DataRecord != nullptr) DataRecord->RemoveRef();
    }
}