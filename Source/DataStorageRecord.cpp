#include "DataStorageRecord.h"

DataStorageRecord::DataStorageRecord() {}

DataStorageRecord::DataStorageRecord(const DataStorageRecord& recordTemplate)
{
    *this = recordTemplate;
    IsDataStorageRecordValid = SmartPointerWrapper<bool> (true);
}

DataStorageRecord::~DataStorageRecord()
{
    IsDataStorageRecordValid.SetData(false);
}


DataStorageRecordRef::DataStorageRecordRef(DataStorageRecord* dataStorageRecord, 
    DataStorageStructureHashMap* dataStorageStructureHashMap, 
    DataStorageStructureMap* dataStorageStructureMap,
    RecursiveReadWriteMutex* dataStorageRecucrsiveReadWriteMtx) :
    DataRecord(dataStorageRecord),
    DataStorageHashMapStructure(dataStorageStructureHashMap), 
    DataStorageMapStructure(dataStorageStructureMap),
    DataStorageRecucrsiveReadWriteMtx(dataStorageRecucrsiveReadWriteMtx)
{
    if (dataStorageRecord != nullptr)
    {
        IsDataStorageRecordValid = dataStorageRecord->IsDataStorageRecordValid;
        DataStorageRecordRecursiveReadWriteMutex = dataStorageRecord->DataStorageRecordRecursiveReadWriteMutex;
    }
    else
    {
        IsDataStorageRecordValid.SetData(false);
    }
}

bool DataStorageRecordRef::operator==(const DataStorageRecordRef& other) const
{
    bool res;
    DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadLock();
    other.DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadLock();
    res = (DataRecord == other.DataRecord);
    other.DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadUnlock();
    DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadUnlock();
    return res;
}

std::string DataStorageRecordRef::GetRecordUniqueId() const
{
    std::stringstream ss;
    DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadLock();
    if (IsValid())
        ss << DataRecord;
    else
        ss << "null";
    DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadUnlock();
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
    bool res;
    DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadLock();
    res = *IsDataStorageRecordValid.GetData();
    DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadUnlock();
    return res;
}

void DataStorageRecordRef::Unlink()
{
    DataRecord = nullptr;
    DataStorageHashMapStructure = nullptr;
    DataStorageMapStructure = nullptr;

    IsDataStorageRecordValid.Unlink();
}