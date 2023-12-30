#include "DataStorage.h"

DataStorage::DataStorage() {}

bool DataStorage::IsKeyExist(const std::string& keyName) const
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

DataStorageRecordRef DataStorage::CreateRecord(const std::vector<std::pair<std::string, DataSaver>>& params)
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

DataStorageRecordSet DataStorage::GetAllRecords() const
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

std::size_t DataStorage::Size() const
{
    return RecordsSet.size();
}

bool DataStorage::SaveToFile(const std::string& fileName) const
{
    // Open or create file to save data
    std::ofstream csvFile(fileName, std::ios::binary);

    // Checking whether the file was opened successfully
    if (!csvFile.is_open())
    {
        std::cout << "Failed to open file!" << std::endl;
        return false;
    }

    // C array to store correct new line characters. 
    const char endOfLine[2] = {13, 10}; // 13 - CR or 0d. 10 - LF or 0a.

    // Save keys to file
    auto it = DataStorageHashMapStructure.cbegin();
    csvFile << it->first;
    ++it;
    for (;it != DataStorageHashMapStructure.cend(); ++it) csvFile << "," << it->first;
    csvFile.write(endOfLine, 2);

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
        csvFile.write(endOfLine, 2);
    }

    csvFile.close();
    return true;
}

bool DataStorage::ReadFromFile(const std::string& fileName)
{
    // Clear old data storage structure
    DropDataStorage();

    // Vector with all records in csv file
    std::vector<std::vector<std::string>> records;
    
    // Try to read csv file
    if (!ReadCsvFile(fileName, records))
        return false;

    // Empty check
    if (records.size() == 0)
        return true;
    
    // Fill keys in data storage
    for (std::size_t i = 0; i < records[0].size(); ++i)
        SetKey<std::string>(records[0][i], "");
    
    DataStorageRecordRef recordRef;

    // Fill records in data storage
    for (std::size_t i = 1; i < records.size(); ++i)
    {
        recordRef = CreateRecord();
        for (std::size_t j = 0; j < records[i].size(); ++j)
            recordRef.SetData(records[0][j], records[i][j]);
    }

    return true;
}

bool DataStorage::ReadFromFile(const std::string& fileName, const std::vector<Putter>& keys)
{
    // Clear old data storage structure
    DropDataStorage();

    // Fill data storage with keys
    for (const Putter& key : keys)
        key.SetKeyToDataStorage(*this);
    
    // Vector with all records in csv file
    std::vector<std::vector<std::string>> records;
    
    // Try to read csv file
    if (!ReadCsvFile(fileName, records))
        return false;

    // Empty check
    if (records.size() == 0)
        return true;

    DataStorageRecordRef recordRef;

    // Fill records in data storage
    // External loop for passing through all records
    for (std::size_t i = 1; i < records.size(); ++i)
    {
        // Creating a new record
        recordRef = CreateRecord();
        // Loop through all the keys that need to be read from the file
        for (std::size_t j = 0; j < keys.size(); ++j)
        { 
            // Loop throw first records element with file key structure
            for (std::size_t k = 0; k < records[0].size(); ++k)
            {
                /// \todo Что делать если такого k нет ?
                // Compare key from file with key from keys vector
                if (records[0][k] == keys[j].GetKey())
                {
                    keys[j].SetRecordData(recordRef, records[i][k]);
                    break;
                }
            }
        }
    }

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


void Putter::SetKeyToDataStorage(DataStorage& dataStorage) const
{
    SetKeyFunc(dataStorage);
}

void Putter::SetRecordData(DataStorageRecordRef& dataStorageRecordRef, const std::string& stringWithData) const
{
    SetRecordDataFunc(dataStorageRecordRef, stringWithData);
}

std::string Putter::GetKey() const
{
    return Key;
}