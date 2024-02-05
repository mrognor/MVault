#include "VaultRecord.h"

namespace mvlt
{
    // VaultRecord

    VaultRecord::VaultRecord()
    {
        Mtx = new std::mutex;
    }

    VaultRecord::VaultRecord(const VaultRecord& other)
    {
        IsValid = true;
        RefCounter = 0;
        Mtx = new std::mutex;

        other.Mtx->lock();
        for (const auto& it : other.Container)
            Container.emplace(it);
        other.Mtx->unlock();
    }

    void VaultRecord::AddRef()
    {
        Mtx->lock();
        if (IsValid) ++RefCounter;
        Mtx->unlock();
    }

    void VaultRecord::RemoveRef()
    {
        bool isEnd = false;
        Mtx->lock();
        --RefCounter;
        if (!IsValid && RefCounter == 0) isEnd = true;
        Mtx->unlock();

        if (isEnd) delete this;
    }

    void VaultRecord::Invalidate()
    {
        bool isEnd = false;
        Mtx->lock();
        IsValid = false;
        if (RefCounter == 0) isEnd = true;
        Mtx->unlock();

        if (isEnd) delete this;
    }

    bool VaultRecord::GetIsValid()
    {
        bool res;
        Mtx->lock();
        res = IsValid;
        Mtx->unlock();
        return res;
    }

    VaultRecord::~VaultRecord()
    {
        delete Mtx;
    }


    // VaultRecord ref

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
        
        std::cout << "Data storage record " << DataRecord << ":" << std::endl;

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