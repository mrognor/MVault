#include "DataStorage.h"

DataStorage::DataStorage() {}

void DataStorage::RemoveKey(const std::string& keyName)
{
    // Erase key from record template
    RecordTemplate.EraseData(keyName);
    // Erase key from DataStorageStructure
    DataStorageStructure.EraseData(keyName);
    // Erase keyh from all maps
    DataStorageRecordAdders.erase(keyName);
    DataStorageRecordClearers.erase(keyName);
    DataStorageRecordErasers.erase(keyName);

    // Erase key data from all records
    for (auto& it : RecordsSet)
        it->EraseData(keyName);
}

DataStorageRecordRef DataStorage::CreateNewRecord()
{
    // Create new record
    DataStorageRecord* newData = new DataStorageRecord(RecordTemplate);
    // Add new record to set
    RecordsSet.emplace(newData);

    // Add new record to every maps inside DataStorageStruct
    for (auto& it : DataStorageRecordAdders)
        it.second(newData);

    return DataStorageRecordRef(newData, &DataStorageStructure);
}

void DataStorage::DropDataStorage()
{
    // Clear record template
    RecordTemplate.Clear();

    // Delete all unordered maps inside DataStorageStructure
    for (auto& it : DataStorageStructure)
        it.second.ResetData();

    // Clear DataStorageStructure
    DataStorageStructure.Clear();

    // Clear all maps with functions
    DataStorageRecordAdders.clear();
    DataStorageRecordClearers.clear();
    DataStorageRecordErasers.clear();

    // Delete all Records
    for (auto& it : RecordsSet)
        delete it;

    // Clear RecordsSet
    RecordsSet.clear();
}

void DataStorage::DropData()
{
    // Call functions to clear DataStorageStructure without
    for (auto& it : DataStorageRecordClearers)
        it.second();

    // Delete all Records
    for (auto& it : RecordsSet)
        delete it;

    // Clear RecordsSet
    RecordsSet.clear();
}

void DataStorage::EraseRecord(const DataStorageRecordRef& recordRefToErase)
{
    // Get pointer to record from record ref
    DataStorageRecord* tmpRec = recordRefToErase.DataRecord;
    // Iterate over all DataStorageRecordErasers and call function to erase record from structure
    for (auto& erasers : DataStorageRecordErasers)
        erasers.second(tmpRec);

    RecordsSet.erase(tmpRec);
    delete tmpRec;
}

void DataStorage::EraseRecords(const DataStorageRecordSet& recordsToErase)
{
    for (auto& recordRef : recordsToErase)
        EraseRecord(recordRef);
}

std::size_t DataStorage::Size()
{
    return RecordsSet.size();
}

DataStorage::~DataStorage()
{
    // Clear DataStorageStructure
    for (auto& it : DataStorageStructure)
        it.second.ResetData();

    // Clear all records
    for (auto& it : RecordsSet)
        delete it;
}