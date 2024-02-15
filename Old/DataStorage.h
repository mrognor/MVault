#pragma once

#include <functional>
#include <vector>

#include "DataStorageClasses.h"
#include "DataSaver.h"
#include "SmartPointerWrapper.h"
#include "DataContainer.h"
#include "DataStorageRecord.h"
#include "DataStorageRecordSet.h"
#include "CsvParser.h"
#include "DataStorageRequests.h"

/**
    \brief A class for storing data with the ability to quickly search for a variety of different keys of any type

    The class can be represented as a simple table with the ability to quickly search in O(1) for each field of the table.
    Each row of the table is called a record, and the DataStorageRecord class is used to store it. The column is called the key.
    Further, the keys will mean the columns of the table, and the records will mean the rows of the table.
    The SetKey function is used to add new keys. There is a template record inside the class and when adding a 
    key to the DataStorage, data will be added to this template record. When creating new records, they will be copied from this template record.
    Each record is unique, but the key values can be the same for many records.
    To work with records inside the DataStorage, the DataStorageRecordRef is used. You can use it to change the values of records inside the DataStorage.
*/
class DataStorage
{
private:

    // Template of data stored inside DataStorage
    DataStorageRecord RecordTemplate;

    /**
        A simple typedef for HashMap. It is necessary for a more understandable separation of types.
        Represents the internal structure of the DataStorage.
        A string with the name of the key is used as the key. All keys are the same as in DataStorage.
        The value stores a pointer to std::unordered_multimap<T, DataStorageRecord*>.
        The key type is same as the DataStorage key value type.
        The value is a pointer to DataStorageRecord.

        Such a complex structure is needed to quickly, in O(1), search for each key with any type.
    */
    mutable DataStorageStructureHashMap DataStorageHashMapStructure;

    /**
        A simple typedef for Map. It is necessary for a more understandable separation of types.
        Represents the internal structure of the DataStorage.
        A string with the name of the key is used as the key. All keys are the same as in DataStorage.
        The value stores a pointer to std::multimap<T, DataStorageRecord*>.
        The key type is same as the DataStorage key value type.
        The value is a pointer to DataStorageRecord.

        Such a complex structure is necessary in order to quickly, in O(log n), 
        find a set of elements that meet a certain requirement, for example, more than a certain value or less than this value
    */
    mutable DataStorageStructureMap DataStorageMapStructure;

    // unordered_map of functions that add a new element to the DataStorageStructureHashMap
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

    /**
        \brief Template function to add new key with default value to DataStorage

        \tparam <T> Any type of data except for c arrays

        If the key was added earlier, the default value will be updated when this function is called again

        \param [in] keyName new key name
        \param [in] defaultKeyValue default key value
    */
    template <class T>
    void SetKey(const std::string& keyName, const T& defaultKeyValue)
    {
        // Add data to template
        RecordTemplate.SetData(keyName, defaultKeyValue);

        // Create new hash map to store data with template T key
        std::unordered_multimap<T, DataStorageRecord*>* TtoDataStorageRecordHashMap = new std::unordered_multimap<T, DataStorageRecord*>;
        DataStorageHashMapStructure.SetData(keyName, TtoDataStorageRecordHashMap, [](const void* ptr)
            {
                delete* (std::unordered_multimap<T, DataStorageRecord*>**)ptr;
            }
        );

        // Create new map to store data with template T key
        std::multimap<T, DataStorageRecord*>* TtoDataStorageRecordMap = new std::multimap<T, DataStorageRecord*>;
        DataStorageMapStructure.SetData(keyName, TtoDataStorageRecordMap, [](const void* ptr)
            {
                delete* (std::multimap<T, DataStorageRecord*>**)ptr;
            }
        );

        // Add function to DataStorageRecord creation
        DataStorageRecordAdders.emplace(keyName, [=](DataStorageRecord* newRecord)
            {
                // Make temporary variable to store key value
                T value = defaultKeyValue;
                // Try to get key value from new record. If it is not value inside then defaultKeyValue will be used
                newRecord->GetData(keyName, value);
                TtoDataStorageRecordHashMap->emplace(value, newRecord);
                TtoDataStorageRecordMap->emplace(value, newRecord);
            }
        );

        // Add function to TtoDataStorageRecordHashMap cleareing
        DataStorageRecordClearers.emplace(keyName, [=]()
            {
                TtoDataStorageRecordHashMap->clear();
                TtoDataStorageRecordMap->clear();
            }
        );

        // Add function to erase newRecord from TtoDataStorageRecordHashMap
        DataStorageRecordErasers.emplace(keyName, [=](DataStorageRecord* newRecord)
            {
                // Get T type data with keyName key
                T recordTData;
                newRecord->GetData(keyName, recordTData);

                // Find all elements on multi_map with recordTData value
                auto FirstAndLastIteratorsWithKeyOnHashMap = TtoDataStorageRecordHashMap->equal_range(recordTData);

                // Find newRecord and erase it from TtoDataStorageRecordHashMap
                for (auto& it = FirstAndLastIteratorsWithKeyOnHashMap.first; it != FirstAndLastIteratorsWithKeyOnHashMap.second; ++it)
                {
                    if (it->second == newRecord)
                    {
                        TtoDataStorageRecordHashMap->erase(it);
                        break;
                    }
                }

                // Find all elements on map with recordTData value
                auto FirstAndLastIteratorsWithKeyOnMap = TtoDataStorageRecordMap->equal_range(recordTData);
                // Find newRecord and erase it from TtoDataStorageRecordHashMap
                for (auto& it = FirstAndLastIteratorsWithKeyOnMap.first; it != FirstAndLastIteratorsWithKeyOnMap.second; ++it)
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
            it->SetData(keyName, defaultKeyValue);
            TtoDataStorageRecordHashMap->emplace(defaultKeyValue, it);
            TtoDataStorageRecordMap->emplace(defaultKeyValue, it);
        }
    }

    /**
        \brief The method for getting a default key value

        \param [in] keyName the name of the key to search for

        \return returns true if the key was found otherwise returns false
    */
    bool IsKeyExist(const std::string& keyName) const;
    
    /**
        \brief The method for getting a default key value

        \tparam <T> Any type of data except for c arrays

        \param [in] keyName the name of the key to search for
        \param [in] keyValue the value of the key

        \return returns true if the key was found otherwise returns false
    */
    template <class T>
    bool GetKeyValue(const std::string& keyName, T& defaultKeyValue) const
    {
        return RecordTemplate.GetData(keyName, defaultKeyValue);
    }

    /// \brief The method for deleting the key
    /// \param [in] keyName the key to remove
    void RemoveKey(const std::string& keyName);

    /// \brief Method to create new DataStorageRecord. A record will be created by copying RecordTemplate.
    /// \return ref to new record 
    DataStorageRecordRef CreateRecord();

    /**
        \brief Method to create new DataStorageRecord.

        The method accepts a vector of pairs, the first element of the pair is a string with a key, 
        and the second element of the pair are key values of any type.
        The order of the pairs is not important, assignment takes place by key. The number of pairs can be any, 
        for all keys for which no value has been specified, the default value will remain

        The DataStorage in the example has 2 keys. One is the id of the int type, and the second is the name of the std::string type
        Usage example:

        \code
            // A record with id 0 and name mrognor will be created
            ds.CreateRecord({ {"id", 0}, {"name", std::string("mrognor")} });
        \endcode

        or

        \code
            // A record with id 0 and name mrognor will be created
            ds.CreateRecord({ {"name", std::string("mrognor")}, {"id", 0} });
        \endcode

        or

        \code
            // A record with name mrognor will be created. The Id will be set to the default value
            ds.CreateRecord({ {"name", std::string("mrognor")} });
        \endcode

        or

        \code
            // A record with id 0 and name mrognor will be created
            std::vector<std::pair<std::string, DataSaver>> params = { {"id", 0}, {"name", std::string("mrognor")} };
            DataStorageRecordRef dsrr = ds.CreateRecord(params);
        \endcode

        what is equivalent to such a code without passing values to a function

        \code
            DataStorageRecordRef dsrr = ds.CreateRecord();

            dsrr.SetData("id", 0);
            dsrr.SetData<std::string>("name", "mrognor");
        \endcode

        \param [in] params a vector of pairs with data to be put in the DataStorage

        \return ref to new record 
    */
    DataStorageRecordRef CreateRecord(const std::vector<std::pair<std::string, DataSaver>>& params);

    /**
        \brief The method for getting a reference to the data inside DataStorage

        \tparam <T> Any type of data except for c arrays

        \param [in] keyName the name of the key to search for
        \param [in] keyValue the value of the key to be found
        \param [out] foundedRecord a ref to the DataStorageRecordRef where found record will be placed

        \return ref to requested record 
    */
    template <class T>
    DataStorageRecordRef GetRecord(const std::string& keyName, const T& keyValue) const
    {
        // Pointer to store map inside DataStorageStructureHashMap
        std::unordered_multimap<T, DataStorageRecord*>* TtoDataStorageRecordHashMap = nullptr;

        DataStorageRecordRef res;

        // Checking whether such a key exists
        if (DataStorageHashMapStructure.GetData(keyName, TtoDataStorageRecordHashMap))
        {
            // Iterator to element with T type and keyValue value
            auto TtoDataStorageRecordIt = TtoDataStorageRecordHashMap->find(keyValue);
            if (TtoDataStorageRecordIt != TtoDataStorageRecordHashMap->end())
            {
                // Set data to DataStorageRecordRef
                res.DataRecord = TtoDataStorageRecordIt->second;
                res.DataStorageHashMapStructure = &DataStorageHashMapStructure;
                res.DataStorageMapStructure = &DataStorageMapStructure;
                res.IsDataStorageRecordValid = true;
                return res;
            }
        }

        res.IsDataStorageRecordValid = false;
        return res;
    }

    /**
        \brief The method for getting a set of references to the data inside DataStorage

        \tparam <T> Any type of data except for c arrays

        \param [in] keyName the name of the key to search for
        \param [in] keyValue the value of the key to be found
        \param [out] foundedRecords a ref to the DataStorageRecordSet where all found records will be placed

        \return returns true if the key was found otherwise returns false
    */
    template <class T>
    bool GetRecords(const std::string& keyName, const T& keyValue, DataStorageRecordSet& foundedRecords) const
    {
        // Pointer to store map inside DataStorageStructureHashMap
        std::unordered_multimap<T, DataStorageRecord*>* TtoDataStorageRecordHashMap = nullptr;

        // Checking whether such a key exists
        if (DataStorageHashMapStructure.GetData(keyName, TtoDataStorageRecordHashMap))
        {
            // Iterator to element with T type and keyValue value
            auto FirstAndLastIteratorsWithKey = TtoDataStorageRecordHashMap->equal_range(keyValue);

            foundedRecords.Clear();

            // Fill the result record set
            for (auto& it = FirstAndLastIteratorsWithKey.first; it != FirstAndLastIteratorsWithKey.second; ++it)
                foundedRecords.AddNewRecord(it->second, &DataStorageHashMapStructure, &DataStorageMapStructure);

            return true;
        }

        return false;
    }

    /**
        \brief The method for getting a set of references to the data inside DataStorage

        \tparam <T> Any type of data except for c arrays

        \param [in] keyName the name of the key to search for
        \param [in] request request for data
        \param [out] foundedRecords a ref to the DataStorageRecordSet where all found records will be placed

        \return returns true if the key was found otherwise returns false
    */
    template <class T>
    bool RequestRecords(const std::string& keyName, const DataStorageRequest<T>& request, DataStorageRecordSet& foundedRecords) const
    {
        // Pointer to store map inside DataStorageStructureHashMap
        std::multimap<T, DataStorageRecord*>* TtoDataStorageRecordMap = nullptr;


        // Checking whether such a key exists
        if (DataStorageMapStructure.GetData(keyName, TtoDataStorageRecordMap))
        {
            foundedRecords.Clear();

            auto begAndEndIterators = request.ProcessRequest(TtoDataStorageRecordMap);

            for (auto& it = begAndEndIterators.first; it != begAndEndIterators.second; ++it)
                foundedRecords.AddNewRecord(DataStorageRecordRef(it->second, &DataStorageHashMapStructure, &DataStorageMapStructure));

            return true;
        }

        return false;
    }

    /// \brief The method for getting a set of all references to the data inside DataStorage
    /// \return returns a set of all records refs
    DataStorageRecordSet GetAllRecords() const;

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
    std::size_t Size() const;

    /// \brief A method for saving data to a table file. The file format is csv
    /// \param fileName The file name to save the data, the extension must be specified manually
    /// \return It will return true if it was possible to open the file and write the data, otherwise it will return false
    bool SaveToFile(const std::string& fileName) const;

    /**
        \brief A method for reading data from a csv file and saving it to a DataStorage
        
        This method deletes all old keys and reads new ones from the file. 
        It treats the first line in the csv file as a list of keys. 
        The string is used as the key type.

        \param [in] fileName The file name to save the data, the extension must be specified manually
        
        \return It will return true if it was possible to open the file and read the data, otherwise it will return false
    */
    bool ReadFromFile(const std::string& fileName);
    
    /**
        \brief A method for reading data from a csv file and saving it to a DataStorage
        
        This method deletes all old keys and reads new ones from the file. 
        It treats the first line in the csv file as a list of keys. 

        \param [in] fileName The file name to save the data, the extension must be specified manually
        \param [in] keys A vector with the keys and the types they store, which should be obtained from the file.
        
        \return It will return true if it was possible to open the file and read the data, otherwise it will return false
    */
    bool ReadFromFile(const std::string& fileName, const std::vector<Putter>& keys);

    /// Default destructor 
    ~DataStorage();
};

/// \todo Подумать над названием этого класса
class Putter
{
private:

    std::function<void(DataStorage&)> SetKeyFunc;

    std::function<void(DataStorageRecordRef&, const std::string&)> SetRecordDataFunc;

    std::string Key;
public:

    template <class T>
    Putter(const std::string& key, const T& defaultKeyValue) : Key(key)
    {
        SetKeyFunc = [=](DataStorage& dataStorage)
            {
                dataStorage.SetKey(key, defaultKeyValue);
            };
        
        SetRecordDataFunc = [=](DataStorageRecordRef& dataStorageRecordRef, const std::string& stringWithData)
            {
                T t;
                if (FromString<T>(stringWithData, t))
                {
                    dataStorageRecordRef.SetData(key, t);
                }
            };
    }

    void SetKeyToDataStorage(DataStorage& dataStorage) const;

    void SetRecordData(DataStorageRecordRef& dataStorageRecordRef, const std::string& stringWithData) const;

    std::string GetKey() const;
};