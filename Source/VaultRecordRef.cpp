#include "VaultRecordRef.h"

namespace mvlt 
{
    VaultRecordRef::VaultRecordRef() {}

    VaultRecordRef::VaultRecordRef(VaultRecord* vaultRecord, 
        VaultStructureHashMap* vaultStructureHashMap, 
        VaultStructureMap* vaultStructureMap,
        RecursiveReadWriteMutex* vaultRecucrsiveReadWriteMtx)
    {
        SetRecord(vaultRecord, vaultStructureHashMap, vaultStructureMap, vaultRecucrsiveReadWriteMtx);
    }

    void VaultRecordRef::SetRecord(VaultRecord* vaultRecord, 
        VaultStructureHashMap* vaultStructureHashMap, 
        VaultStructureMap* vaultStructureMap,
        RecursiveReadWriteMutex* vaultRecucrsiveReadWriteMtx)
    {
        vaultRecucrsiveReadWriteMtx->ReadLock();
        if (vaultRecord != nullptr) vaultRecord->AddRef();
        DataRecord = vaultRecord;
        VaultHashMapStructure = vaultStructureHashMap;
        VaultMapStructure = vaultStructureMap;
        VaultRecucrsiveReadWriteMtx = vaultRecucrsiveReadWriteMtx;
        vaultRecucrsiveReadWriteMtx->ReadUnlock();
    }

    VaultRecordRef::VaultRecordRef(const VaultRecordRef& other)
    {
        *this = other;
    }

    VaultRecordRef& VaultRecordRef::operator=(const VaultRecordRef& other)
    {
        if (&other != this)
        {
            other.VaultRecucrsiveReadWriteMtx->ReadLock();
            if (DataRecord != nullptr) DataRecord->RemoveRef();
            if (other.DataRecord != nullptr) other.DataRecord->AddRef();
            DataRecord = other.DataRecord;
            VaultHashMapStructure = other.VaultHashMapStructure;
            VaultMapStructure = other.VaultMapStructure;
            VaultRecucrsiveReadWriteMtx = other.VaultRecucrsiveReadWriteMtx;
            other.VaultRecucrsiveReadWriteMtx->ReadUnlock();
        }
        
        return *this;
    }

    bool VaultRecordRef::operator==(const VaultRecordRef& other) const
    {
        bool res;
        VaultRecucrsiveReadWriteMtx->ReadLock();
        res = (DataRecord == other.DataRecord);
        VaultRecucrsiveReadWriteMtx->ReadUnlock();
        return res;
    }

    std::string VaultRecordRef::GetRecordUniqueId() const
    {
        std::stringstream ss;
        VaultRecucrsiveReadWriteMtx->ReadLock();
        if (IsValid())
            ss << DataRecord;
        else
            ss << "null";
        VaultRecucrsiveReadWriteMtx->ReadUnlock();
        return ss.str();
    }

    void VaultRecordRef::SetData(const std::vector<std::pair<std::string, DataSaver>>& params)
    {
        /// \todo Это вообще не правильно
        // Copy data from function parametrs
        for (auto& it : params)
            if (DataRecord->IsData(it.first))
                DataRecord->SetDataFromDataSaver(it.first, it.second);
    }

    bool VaultRecordRef::GetDataAsString(const std::string& key, std::string& str) const
    {
        bool res;
        VaultRecucrsiveReadWriteMtx->ReadLock();
        if (DataRecord->GetIsValid()) res = DataRecord->GetDataAsString(key, str);
        VaultRecucrsiveReadWriteMtx->ReadUnlock();
        return res;
    }

    bool VaultRecordRef::IsValid() const
    {
        // Thread saferty
        if (DataRecord != nullptr)
            return DataRecord->GetIsValid();
        else
            return false;
    }

    std::vector<std::string> VaultRecordRef::GetKeys() const
    {
        std::vector<std::string> res;

        VaultRecucrsiveReadWriteMtx->ReadLock();
        for (const auto& it : *VaultMapStructure) res.emplace_back(it.first);
        VaultRecucrsiveReadWriteMtx->ReadUnlock();

        return res;
    }

    void VaultRecordRef::PrintRecord() const
    {
        VaultRecucrsiveReadWriteMtx->ReadLock();
        
        std::cout << "Vault record " << DataRecord << ":" << std::endl;

        for (const auto& keyPair : *VaultHashMapStructure)
        {
            DataSaver dataSaver;
            DataRecord->GetDataSaver(keyPair.first, dataSaver);
        
            std::cout << "\t" << keyPair.first << " = " << dataSaver.Str() << std::endl;
        }

        VaultRecucrsiveReadWriteMtx->ReadUnlock();
    }

    void VaultRecordRef::Unlink()
    {
        DataRecord = nullptr;
        VaultHashMapStructure = nullptr;
        VaultMapStructure = nullptr;
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