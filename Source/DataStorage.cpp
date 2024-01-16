#include "DataStorage.h"
#include "DataSaver.h"

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

std::vector<std::string> DataStorage::GetKeys() const
{
    std::vector<std::string> res;

    RecursiveReadWriteMtx.ReadLock();
    for (const auto& it : DataStorageMapStructure) res.emplace_back(it.first);
    RecursiveReadWriteMtx.ReadUnlock();
    
    return res;
}

void DataStorage::PrintDataStorage(const std::size_t amountOfRecords) const
{
    RecursiveReadWriteMtx.ReadLock();

    std::vector<std::string> keys = GetKeys();
    std::size_t counter = 0;

    for (const auto& record : RecordsSet)
    {
        std::cout << "Data storage record " << record << ":" << std::endl;

        for (const std::string& key : keys)
        {
            DataSaver dataSaver;
            record->GetDataSaver(key, dataSaver);

            std::cout << "\t" << key << " = " << dataSaver.Str() << std::endl;
        }

        ++counter;
        if (counter == amountOfRecords) break;
    }

    RecursiveReadWriteMtx.ReadUnlock();
}

void DataStorage::PrintAsTable(const std::size_t amountOfRecords) const
{
    RecursiveReadWriteMtx.ReadLock();
    
    std::vector<std::string> keys = GetKeys();
    std::vector<std::size_t> maxLengths(keys.size());

    // Fill each max length using key length
    for (std::size_t i = 0; i < keys.size(); ++i)
        maxLengths[i] = keys[i].length();
    
    // Find each real max length
    for (std::size_t i = 0; i < keys.size(); ++i)
    {
        for (const auto& record : RecordsSet)
        {
            DataSaver dataSaver;
            record->GetDataSaver(keys[i], dataSaver);
            if (dataSaver.Str().length() > maxLengths[i]) maxLengths[i] = dataSaver.Str().length();
        }
    }

    // Print splitter
    std::cout << "+";
    for (std::size_t i = 0; i < keys.size(); ++i)
    {
        std::cout << "-";
        for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "-";
        std::cout << "-+";
    }
    std::cout << std::endl;

    // Print header
    std::cout << "|";
    for (std::size_t i = 0; i < keys.size(); ++i)
    {
        std::cout << " " << keys[i];
        for (std::size_t j = keys[i].length(); j < maxLengths[i]; ++j) std::cout << " ";
        std::cout << " |";
    }
    std::cout << std::endl;

    // Print splitter
    std::cout << "+";
    for (std::size_t i = 0; i < keys.size(); ++i)
    {
        std::cout << "-";
        for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "-";
        std::cout << "-+";
    }
    std::cout << std::endl;

    // Print records
    std::size_t counter = 0;
    for (const auto& record : RecordsSet)
    {
        std::cout << "|";
        for (std::size_t i = 0; i < keys.size(); ++i)
        {
            DataSaver dataSaver;
            record->GetDataSaver(keys[i], dataSaver);
            std::cout << " " << dataSaver.Str();
            for (std::size_t j = dataSaver.Str().length(); j < maxLengths[i]; ++j) std::cout << " ";
            std::cout << " |";
        }
        std::cout << std::endl;
        
        ++counter;
        if (counter == amountOfRecords) break;
    }

    // Print splitter
    std::cout << "+";
    for (std::size_t i = 0; i < keys.size(); ++i)
    {
        std::cout << "-";
        for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "-";
        std::cout << "-+";
    }
    std::cout << std::endl;

    std::cout << " (" << Size() << " records)" << std::endl;

    RecursiveReadWriteMtx.ReadUnlock();
}

DataStorage::~DataStorage()
{
    RecursiveReadWriteMtx.WriteLock();
    
    // Clear DataStorageHashMapStructure
    for (auto& it : DataStorageHashMapStructure)
        it.second.ResetData();

    // Clear DataStorageMapStructure
    for (auto& it : DataStorageMapStructure)
        it.second.ResetData();

    // Clear all records
    for (auto& it : RecordsSet)
        it->Invalidate();

    RecursiveReadWriteMtx.WriteUnlock();
}