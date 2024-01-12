#include "DataStorage.h"

DataStorage::DataStorage() {}

bool DataStorage::IsKeyExist(const std::string& keyName) const
{
    bool res;
    RecursiveReadWriteMtx.ReadLock();
    res = RecordTemplate.IsData(keyName);
    RecursiveReadWriteMtx.ReadUnlock();
    return res;
}

void DataStorage::RemoveKey(const std::string& keyName)
{
    RecursiveReadWriteMtx.WriteLock();
    if (!IsKeyExist(keyName)) 
    {
        RecursiveReadWriteMtx.WriteUnlock();
        return;
    }

    // Erase key from record template
    RecordTemplate.EraseData(keyName);

    // Erase key from DataStorageHashMapStructure
    DataStorageHashMapStructure.EraseData(keyName);

    // Erase key from DataStorageMapStructure
    DataStorageMapStructure.EraseData(keyName);

    // Erase key from all maps
    DataStorageRecordAdders.erase(keyName);
    DataStorageRecordClearers.erase(keyName);
    DataStorageRecordErasers.erase(keyName);

    // Erase key data from all records
    for (auto& it : RecordsSet)
        it->EraseData(keyName);
    
    RecursiveReadWriteMtx.WriteUnlock();
}

DataStorageRecordRef DataStorage::CreateRecord()
{
    RecursiveReadWriteMtx.WriteLock();

    // Create new record
    DataStorageRecord* newData = new DataStorageRecord(RecordTemplate);
    // Add new record to set
    RecordsSet.emplace(newData);

    // Add new record to every maps inside DataStorageStructureHashMap
    for (auto& it : DataStorageRecordAdders)
        it.second(newData);

    DataStorageRecordRef res(newData, &DataStorageHashMapStructure, &DataStorageMapStructure, &RecursiveReadWriteMtx);

    RecursiveReadWriteMtx.WriteUnlock();

    return res;
}

DataStorageRecordRef DataStorage::CreateRecord(const std::vector<std::pair<std::string, DataSaver>>& params)
{
    RecursiveReadWriteMtx.WriteLock();

    // Create new record
    DataStorageRecord* newData = new DataStorageRecord(RecordTemplate);

    // Copy data from function parametrs
    for (auto& it : params)
        if (newData->IsData(it.first))
            newData->SetDataFromDataSaver(it.first, it.second);

    // Add new record to set
    RecordsSet.emplace(newData);

    // Add new record to every maps inside DataStorageStructureHashMap
    for (auto& it : DataStorageRecordAdders)
        it.second(newData);
    
    DataStorageRecordRef res(newData, &DataStorageHashMapStructure, &DataStorageMapStructure, &RecursiveReadWriteMtx);
    
    RecursiveReadWriteMtx.WriteUnlock();

    return res;
}

void DataStorage::DropDataStorage()
{
    RecursiveReadWriteMtx.WriteLock();

    // Clear record template
    RecordTemplate.Clear();

    // Delete all unordered maps inside DataStorageHashMapStructure
    for (auto& it : DataStorageHashMapStructure)
        it.second.ResetData();

    // Clear DataStorageHashMapStructure
    DataStorageHashMapStructure.Clear();

    // Delete all maps inside DataStorageMapStructure
    for (auto& it : DataStorageMapStructure)
        it.second.ResetData();

    // Clear DataStorageMapStructure
    DataStorageMapStructure.Clear();


    // Clear all maps with functions
    DataStorageRecordAdders.clear();
    DataStorageRecordClearers.clear();
    DataStorageRecordErasers.clear();

    // Delete all Records
    for (auto& it : RecordsSet)
        it->Invalidate();

    // Clear RecordsSet
    RecordsSet.clear();

    RecursiveReadWriteMtx.WriteUnlock();
}

void DataStorage::DropData()
{
    RecursiveReadWriteMtx.WriteLock();

    // Call functions to clear DataStorageHashMapStructure without
    for (auto& it : DataStorageRecordClearers)
        it.second();

    // Delete all Records
    for (auto& it : RecordsSet)
        it->Invalidate();

    // Clear RecordsSet
    RecordsSet.clear();

    RecursiveReadWriteMtx.WriteUnlock();
}

void DataStorage::EraseRecord(const DataStorageRecordRef& recordRefToErase)
{
    RecursiveReadWriteMtx.WriteLock();

    if (RecordsSet.count(recordRefToErase.DataRecord) == 0)
    {
        RecursiveReadWriteMtx.WriteUnlock();
        return;
    }

    // Get pointer to record from record ref
    DataStorageRecord* tmpRec = recordRefToErase.DataRecord;
    // Iterate over all DataStorageRecordErasers and call function to erase record from structure
    for (auto& erasers : DataStorageRecordErasers)
        erasers.second(tmpRec);

    RecordsSet.erase(tmpRec);
    tmpRec->Invalidate();

    RecursiveReadWriteMtx.WriteUnlock();
}

std::size_t DataStorage::Size() const
{
    std::size_t res;
    RecursiveReadWriteMtx.ReadLock();
    res = RecordsSet.size();
    RecursiveReadWriteMtx.ReadUnlock();
    return res;
}

DataStorage::~DataStorage()
{
    // Clear DataStorageHashMapStructure
    for (auto& it : DataStorageHashMapStructure)
        it.second.ResetData();

    // Clear DataStorageMapStructure
    for (auto& it : DataStorageMapStructure)
        it.second.ResetData();

    // Clear all records
    for (auto& it : RecordsSet)
        it->Invalidate();
}