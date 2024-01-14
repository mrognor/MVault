#include "DataStorageRecord.h"

// DataStorageRecord

DataStorageRecord::DataStorageRecord()
{
    Mtx = new std::mutex;
}

DataStorageRecord::DataStorageRecord(const DataStorageRecord& other)
{
    IsValid = true;
    RefCounter = 0;
    Mtx = new std::mutex;

    other.Mtx->lock();
    for (const auto& it : other.Container)
        Container.emplace(it);
    other.Mtx->unlock();
}

void DataStorageRecord::AddRef()
{
    Mtx->lock();
    if (IsValid) ++RefCounter;
    Mtx->unlock();
}

void DataStorageRecord::RemoveRef()
{
    bool isEnd = false;
    Mtx->lock();
    --RefCounter;
    if (!IsValid && RefCounter == 0) isEnd = true;
    Mtx->unlock();

    if (isEnd) delete this;
}

void DataStorageRecord::Invalidate()
{
    bool isEnd = false;
    Mtx->lock();
    IsValid = false;
    if (RefCounter == 0) isEnd = true;
    Mtx->unlock();

    if (isEnd) delete this;
}

bool DataStorageRecord::GetIsValid()
{
    bool res;
    Mtx->lock();
    res = IsValid;
    Mtx->unlock();
    return res;
}

DataStorageRecord::~DataStorageRecord()
{
    delete Mtx;
}


// DataStorageRecord ref

DataStorageRecordRef::DataStorageRecordRef(DataStorageRecord* dataStorageRecord, 
    DataStorageStructureHashMap* dataStorageStructureHashMap, 
    DataStorageStructureMap* dataStorageStructureMap,
    RecursiveReadWriteMutex* dataStorageRecucrsiveReadWriteMtx) :
    DataRecord(dataStorageRecord),
    DataStorageHashMapStructure(dataStorageStructureHashMap), 
    DataStorageMapStructure(dataStorageStructureMap),
    DataStorageRecucrsiveReadWriteMtx(dataStorageRecucrsiveReadWriteMtx)
{
    DataStorageRecucrsiveReadWriteMtx->ReadLock();
    if (DataRecord != nullptr) DataRecord->AddRef();
    DataStorageRecucrsiveReadWriteMtx->ReadUnlock();
}

DataStorageRecordRef& DataStorageRecordRef::operator=(const DataStorageRecordRef& other)
{
    if (&other != this)
    {
        other.DataStorageRecucrsiveReadWriteMtx->ReadLock();
        if (DataRecord != nullptr) other.DataRecord->AddRef();
        DataStorageHashMapStructure = other.DataStorageHashMapStructure;
        DataStorageMapStructure = other.DataStorageMapStructure;
        DataStorageRecucrsiveReadWriteMtx = other.DataStorageRecucrsiveReadWriteMtx;
        other.DataStorageRecucrsiveReadWriteMtx->ReadUnlock();
    }
    
    return *this;
}

bool DataStorageRecordRef::operator==(const DataStorageRecordRef& other) const
{
    bool res;
    DataStorageRecucrsiveReadWriteMtx->ReadLock();
    res = (DataRecord == other.DataRecord);
    DataStorageRecucrsiveReadWriteMtx->ReadUnlock();
    return res;
}

std::string DataStorageRecordRef::GetRecordUniqueId() const
{
    std::stringstream ss;
    DataStorageRecucrsiveReadWriteMtx->ReadLock();
    if (IsValid())
        ss << DataRecord;
    else
        ss << "null";
    DataStorageRecucrsiveReadWriteMtx->ReadUnlock();
    return ss.str();
}

void DataStorageRecordRef::SetData(const std::vector<std::pair<std::string, DataSaver>>& params)
{
    /// \todo Это вообще не правильно
    // Copy data from function parametrs
    for (auto& it : params)
        if (DataRecord->IsData(it.first))
            DataRecord->SetDataFromDataSaver(it.first, it.second);
}

bool DataStorageRecordRef::IsValid() const
{
    // Thread saferty
    if (DataRecord != nullptr)
        return DataRecord->GetIsValid();
    else
        return false;
}

void DataStorageRecordRef::Unlink()
{
    DataRecord = nullptr;
    DataStorageHashMapStructure = nullptr;
    DataStorageMapStructure = nullptr;
}

DataStorageRecordRef::~DataStorageRecordRef()
{
    DataStorageRecucrsiveReadWriteMtx->ReadLock();
    if (DataRecord != nullptr) DataRecord->RemoveRef();
    DataStorageRecucrsiveReadWriteMtx->ReadUnlock();
}