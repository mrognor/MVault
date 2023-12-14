#include "DataStorage.h"

DataStorage::DataStorage() {}

bool DataStorage::IsKeyExist(const std::string& keyName)
{
    return RecordTemplate.IsData(keyName);
}

void DataStorage::RemoveKey(const std::string& keyName)
{
    if (!IsKeyExist(keyName)) return;

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
}

DataStorageRecordRef DataStorage::CreateRecord()
{
    // Create new record
    DataStorageRecord* newData = new DataStorageRecord(RecordTemplate);
    // Add new record to set
    RecordsSet.emplace(newData);

    // Add new record to every maps inside DataStorageStructureHashMap
    for (auto& it : DataStorageRecordAdders)
        it.second(newData);

    return DataStorageRecordRef(newData, &DataStorageHashMapStructure, &DataStorageMapStructure);
}

DataStorageRecordRef DataStorage::CreateRecord(std::vector<std::pair<std::string, DataSaver>> params)
{
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
    
    return DataStorageRecordRef(newData, &DataStorageHashMapStructure, &DataStorageMapStructure);
}

DataStorageRecordSet DataStorage::GetAllRecords()
{
    DataStorageRecordSet res;
    // Fill the result record set
    for (auto it = RecordsSet.begin(); it != RecordsSet.end(); ++it)
        res.AddNewRecord(*it, &DataStorageHashMapStructure, &DataStorageMapStructure);

    return res;
}

void DataStorage::DropDataStorage()
{
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
        delete it;

    // Clear RecordsSet
    RecordsSet.clear();
}

void DataStorage::DropData()
{
    // Call functions to clear DataStorageHashMapStructure without
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
    if (RecordsSet.count(recordRefToErase.DataRecord) == 0)
        return;

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

bool DataStorage::SaveToFile(const std::string& fileName)
{
    std::ofstream csvFile(fileName);

    if (!csvFile.is_open())
    {
        std::cout << "Failed to open file!" << std::endl;
        return false;
    }

    auto it = DataStorageHashMapStructure.begin();
    csvFile << it->first;
    ++it;
    for (;it != DataStorageHashMapStructure.end(); ++it)
        csvFile << "," << it->first;
    csvFile << std::endl;

    for (const auto& record : RecordsSet)
    {
        // Getting the number of items in a record 
        std::size_t counter = record->Size();
        for (const auto& recordKeyValue : *record)
        {
            // Calling the string formatting function for the csv format
            csvFile << FormatStringToCsv(recordKeyValue.second.Str());
            
            // Checking not to write a comma to the end of the line
            if (counter == 1) break;
            
            csvFile << ",";
            --counter;
        }
        csvFile << std::endl;
    }

    csvFile.close();
    return true;
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
        delete it;
}