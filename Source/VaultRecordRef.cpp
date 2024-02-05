#include "VaultRecordRef.h"

namespace mvlt 
{
    VaultRecordRef::VaultRecordRef(VaultRecord* vaultRecord, 
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

    bool VaultRecordRef::IsValid() const
    {
        // Thread saferty
        if (DataRecord != nullptr)
            return DataRecord->GetIsValid();
        else
            return false;
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

    void VaultRecordRef::WriteLock()
    {
        VaultRecucrsiveReadWriteMtx->WriteLock();
    }

    void VaultRecordRef::WriteUnlock()
    {
        VaultRecucrsiveReadWriteMtx->WriteUnlock();
    }
        
    VaultRecordRef::~VaultRecordRef()
    {
        if (DataRecord != nullptr) DataRecord->RemoveRef();
    }
}