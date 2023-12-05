#pragma once

#include <functional>

#include "DataStorageClasses.h"
#include "DataSaver.h"
#include "SmartPointerWrapper.h"
#include "DataContainer.h"
#include "DataStorageRecord.h"
#include "DataStorageRecordSet.h"

/**
    \brief A class for storing data with the ability to quickly search for a variety of different keys of any type

    The class can be represented as a simple table with the ability to quickly search in O(1) for each field of the table.
    Each row of the table is called a record, and the DataStorageRecord class is used to store it. The column is called the key.
    Further, the keys will mean the columns of the table, and the records will mean the rows of the table.
    The AddKey function is used to add new keys. There is a template entry inside the class and when adding a 
    key to the DataStorage, data will be added to this template entry. When creating new records, they will be copied from this template record.
    Each record is unique, but the key values can be the same for many records.
    To work with records inside the DataStorage, the DataStorageRecordRef is used. You can use it to change the values of records inside the DataStorage.
*/
class DataStorage
{
private:

    // Template of data stored inside DataStorage
    DataStorageRecord RecordTemplate;

    /*
        Data storage structure inside DataStorage
        A string with the name of the key is used as the key. All keys are the same as in DataStorage.
        The value stores a pointer to std::unordered_multimap<T, DataStorageRecord*>.
        Such a complex structure is needed to quickly search for each key with any type.
        The key type is same as the DataStorage key value type.
        The value is a pointer to DataStorageRecord.
    */
    DataStorageStruct DataStorageStructure;

    // unordered_map of functions that add a new element to the DataStorageStruct
    std::unordered_map<std::string, std::function<void(DataStorageRecord*)>> DataStorageRecordAdders;

    // unordered_map of functions that delete all data from the unordered_map's stored in the DataStorageRecordAdders
    std::unordered_map<std::string, std::function<void()>> DataStorageRecordClearers;

    // unordered_map of functions that erase record from the unordered_map's stored in the DataStorageRecordAdders
    std::unordered_map<std::string, std::function<void(DataStorageRecord* newRecord)>> DataStorageRecordErasers;

    // Unordered set with all DataStorageRecord pointers
    std::unordered_set<DataStorageRecord*> RecordsSet;
public:

    /// Default constructor
    DataStorage();

    /// Deleted assign operator
    DataStorage& operator= (const DataStorage& other) = delete;

    /// Deleted copy operator
    DataStorage(const DataStorage& other) = delete;

    /// \brief Template function to add new key with default value to DataStorage
    /// \param [in] keyName new key name
    /// \param [in] defaultKeyValue default key value
    template <class T>
    void AddKey(const std::string& keyName, const T& defaultKeyValue)
    {
        // Add data to template
        RecordTemplate.AddData(keyName, defaultKeyValue);

        // Create new map to store data with template T key
        std::unordered_multimap<T, DataStorageRecord*>* TtoDataStorageRecordMap = new std::unordered_multimap<T, DataStorageRecord*>;
        DataStorageStructure.AddData(keyName, TtoDataStorageRecordMap, [](const void* ptr)
            {
                delete* (std::unordered_multimap<T, DataStorageRecord*>**)ptr;
            }
        );

        // Add function to DataStorageRecord creation
        DataStorageRecordAdders.emplace(keyName, [=](DataStorageRecord* newRecord)
            {
                TtoDataStorageRecordMap->emplace(defaultKeyValue, newRecord);
            }
        );

        // Add function to TtoDataStorageRecordMap cleareing
        DataStorageRecordClearers.emplace(keyName, [=]()
            {
                TtoDataStorageRecordMap->clear();
            }
        );

        // Add function to erase newRecord from TtoDataStorageRecordMap
        DataStorageRecordErasers.emplace(keyName, [=](DataStorageRecord* newRecord)
            {
                // Get T type data with keyName key
                T recordTData;
                newRecord->GetData(keyName, recordTData);

                // Find all elements on multi_map with recordTData value
                auto FirstAndLastIteratorsWithKey = TtoDataStorageRecordMap->equal_range(recordTData);

                // Find newRecord and erase it from TtoDataStorageRecordMap
                for (auto& it = FirstAndLastIteratorsWithKey.first; it != FirstAndLastIteratorsWithKey.second; ++it)
                {
                    if (it->second == newRecord)
                    {
                        TtoDataStorageRecordMap->erase(it);
                        break;
                    }
                }
            }
        );

        // Add new data to record set
        for (auto& it : RecordsSet)
        {
            it->AddData(keyName, defaultKeyValue);
            TtoDataStorageRecordMap->emplace(defaultKeyValue, it);
        }
    }

    /// \brief The method for deleting the key
    /// \param [in] keyName the key to remove
    void RemoveKey(const std::string& keyName);

    /// Method to create new DataStorageRecord. A record will be created by copying RecordTemplate.
    /// \return ref to new record 
    DataStorageRecordRef CreateNewRecord();

    /**
        \brief The method for getting a reference to the data inside DataStorage

        \param [in] keyName the name of the key to search for
        \param [in] keyValue the value of the key to be found
        \param [out] foundedRecord a ref to the DataStorageRecordRef where found record will be placed

        \return returns true if the key and value was found otherwise returns false
    */
    template <class T>
    bool GetRecord(const std::string& keyName, const T& keyValue, DataStorageRecordRef& foundedRecord)
    {
        // Pointer to store map inside DataStorageStruct
        std::unordered_multimap<T, DataStorageRecord*>* TtoDataStorageRecordMap = nullptr;

        // Checking whether such a key exists
        if (DataStorageStructure.GetData(keyName, TtoDataStorageRecordMap))
        {
            // Iterator to element with T type and keyValue value
            auto TtoDataStorageRecordIt = TtoDataStorageRecordMap->find(keyValue);
            if (TtoDataStorageRecordIt != TtoDataStorageRecordMap->end())
            {
                // Set data to DataStorageRecordRef
                foundedRecord.DataRecord = TtoDataStorageRecordIt->second;
                foundedRecord.DataStorageStructure = &DataStorageStructure;
                return true;
            }
        }

        return false;
    }

    /**
        \brief The method for getting a set of references to the data inside DataStorage

        \param [in] keyName the name of the key to search for
        \param [in] keyValue the value of the key to be found
        \param [out] foundedRecords a ref to the DataStorageRecordSet where all found records will be placed

        \return returns true if the key was found otherwise returns false
    */
    template <class T>
    bool GetRecords(const std::string& keyName, const T& keyValue, DataStorageRecordSet& foundedRecords)
    {
        // Pointer to store map inside DataStorageStruct
        std::unordered_multimap<T, DataStorageRecord*>* TtoDataStorageRecordMap = nullptr;

        // Checking whether such a key exists
        if (DataStorageStructure.GetData(keyName, TtoDataStorageRecordMap))
        {
            // Iterator to element with T type and keyValue value
            auto FirstAndLastIteratorsWithKey = TtoDataStorageRecordMap->equal_range(keyValue);

            foundedRecords.Clear();

            // Fill the result record set
            for (auto& it = FirstAndLastIteratorsWithKey.first; it != FirstAndLastIteratorsWithKey.second; ++it)
                foundedRecords.AddNewRecord(it->second, &DataStorageStructure);

            return true;
        }

        return false;
    }

    /// A method for deleting all data and keys
    void DropDataStorage();

    /// A method for deleting all data, but keeping all keys
    void DropData();

    /// \brief Method for deleting a record from a DataStorage
    /// \param recordRefToErase the reference to the record that needs to be deleted
    void EraseRecord(const DataStorageRecordRef& recordRefToErase);

    /// \brief Method for deleting a records from a DataStorage
    /// \param recordsToErase a set of records to delete
    void EraseRecords(const DataStorageRecordSet& recordsToErase);
    
    /// \brief Method for getting the number of records
    /// \return number of records
    std::size_t Size();

    /// Default destructor 
    ~DataStorage();
};
