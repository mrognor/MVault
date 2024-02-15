#pragma once

#include <functional>
#include <vector>

#include "VaultClasses.h"
#include "DataSaver.h"
#include "DataContainer.h"
#include "VaultRecord.h"
#include "VaultRecordRef.h"
#include "ReadWriteMutex.h"
#include "VaultFunctions.h"

namespace mvlt
{
    /**
        \brief A class for storing data with the ability to quickly search for a variety of different keys of any type

        The class can be represented as a simple table with the ability to quickly search in O(1) for each field of the table.
        Each row of the table is called a record, and the VaultRecord class is used to store it. The column is called the key.
        Further, the keys will mean the columns of the table, and the records will mean the rows of the table.
        The SetKey function is used to add new keys. There is a template record inside the class and when adding a 
        key to the Vault, data will be added to this template record. When creating new records, they will be copied from this template record.
        Each record is unique, but the key values can be the same for many records.
        To work with records inside the Vault, the VaultRecordRef is used. You can use it to change the values of records inside the Vault.
    */
    class Vault
    {
    private:

        // Template of data stored inside Vault
        VaultRecord RecordTemplate;

        /*
            A simple typedef for HashMap. It is necessary for a more understandable separation of types.
            Represents the internal structure of the Vault.
            A string with the name of the key is used as the key. All keys are the same as in Vault.
            The value stores a pointer to std::unordered_multimap<T, VaultRecord*>.
            The key type is same as the Vault key value type.
            The value is a pointer to VaultRecord.

            Such a complex structure is needed to quickly, in O(1), search for each key with any type.
        */
        mutable VaultStructureHashMap VaultHashMapStructure;

        /*
            A simple typedef for Map. It is necessary for a more understandable separation of types.
            Represents the internal structure of the Vault.
            A string with the name of the key is used as the key. All keys are the same as in Vault.
            The value stores a pointer to std::multimap<T, VaultRecord*>.
            The key type is same as the Vault key value type.
            The value is a pointer to VaultRecord.

            Such a complex structure is necessary in order to quickly, in O(log n), 
            find a set of elements that meet a certain requirement, for example, more than a certain value or less than this value
        */
        mutable VaultStructureMap VaultMapStructure;

        // Unordered_map of functions that add a new element to the VaultStructureHashMap
        std::unordered_map<std::string, std::function<void(VaultRecord*)>> VaultRecordAdders;

        // Unordered_map of functions that delete all data from the unordered_map's
        std::unordered_map<std::string, std::function<void()>> VaultRecordClearers;

        // Unordered_map of functions that erase record from the unordered_map's
        std::unordered_map<std::string, std::function<void(VaultRecord* newRecord)>> VaultRecordErasers;

        // Unordered_map of functions for getting sorted data.
        // The key is a string with the name of the key from the Vault.
        // The std::function is used as the value, in which the lambda function is written at the time of adding the key.
        // Lambda accepts a function that is called for each record inside. VaultRecordRef is passed to her. 
        // By default, iteration by records occurs in ascending order. 
        // isReverse parameter is used for the reverse order.
        std::unordered_map<std::string, std::function<void( std::function<bool(const VaultRecordRef&)> functionToSortedData, bool isReverse )>> VaultRecordSorters;

        // Unordered set with all VaultRecord pointers
        std::unordered_set<VaultRecord*> RecordsSet;

        // Recursive mutex for thread safety
        mutable RecursiveReadWriteMutex RecursiveReadWriteMtx;

    public:

        /// \brief Default constructor
        Vault();

        /// \brief Deleted copy constructor
        Vault(const Vault& other) = delete;

        /// \brief Deleted assign operator
        Vault& operator= (const Vault& other) = delete;

        /**
            \brief Template function to add new key with default value to Vault

            \tparam <T> Any type of data except for c arrays

            If the key was added earlier, the default value will be updated when this function is called again

            \param [in] keyName new key name
            \param [in] defaultKeyValue default key value
        */
        template <class T>
        void SetKey(const std::string& keyName, const T& defaultKeyValue)
        {
            RecursiveReadWriteMtx.WriteLock();

            // If the key was added earlier, then it must be deleted
            if (IsKeyExist(keyName))
                RemoveKey(keyName);

            // Add data to template
            RecordTemplate.SetData(keyName, defaultKeyValue);

            // Create new hash map to store data with template T key
            std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = new std::unordered_multimap<T, VaultRecord*>;
            VaultHashMapStructure.SetData(keyName, TtoVaultRecordHashMap, [](const void* ptr)
                {
                    delete* (std::unordered_multimap<T, VaultRecord*>**)ptr;
                }
            );

            // Create new map to store data with template T key
            std::multimap<T, VaultRecord*>* TtoVaultRecordMap = new std::multimap<T, VaultRecord*>;
            VaultMapStructure.SetData(keyName, TtoVaultRecordMap, [](const void* ptr)
                {
                    delete* (std::multimap<T, VaultRecord*>**)ptr;
                }
            );

            // Add function to VaultRecord creation
            VaultRecordAdders.emplace(keyName, [=](VaultRecord* newRecord)
                {
                    // Make temporary variable to store key value
                    T value = defaultKeyValue;
                    // Try to get key value from new record. If it is not value inside then defaultKeyValue will be used
                    newRecord->GetData(keyName, value);
                    TtoVaultRecordHashMap->emplace(value, newRecord);
                    TtoVaultRecordMap->emplace(value, newRecord);
                }
            );

            // Add function to TtoVaultRecordHashMap cleareing
            VaultRecordClearers.emplace(keyName, [=]()
                {
                    TtoVaultRecordHashMap->clear();
                    TtoVaultRecordMap->clear();
                }
            );

            // Add function to erase newRecord from TtoVaultRecordHashMap
            VaultRecordErasers.emplace(keyName, [=](VaultRecord* newRecord)
                {
                    // Get T type data with keyName key
                    T recordTData;
                    newRecord->GetData(keyName, recordTData);

                    // Find all elements on multi_map with recordTData value
                    auto FirstAndLastIteratorsWithKeyOnHashMap = TtoVaultRecordHashMap->equal_range(recordTData);

                    // Find newRecord and erase it from TtoVaultRecordHashMap
                    for (auto& it = FirstAndLastIteratorsWithKeyOnHashMap.first; it != FirstAndLastIteratorsWithKeyOnHashMap.second; ++it)
                    {
                        if (it->second == newRecord)
                        {
                            TtoVaultRecordHashMap->erase(it);
                            break;
                        }
                    }

                    // Find all elements on map with recordTData value
                    auto FirstAndLastIteratorsWithKeyOnMap = TtoVaultRecordMap->equal_range(recordTData);
                    // Find newRecord and erase it from TtoVaultRecordHashMap
                    for (auto& it = FirstAndLastIteratorsWithKeyOnMap.first; it != FirstAndLastIteratorsWithKeyOnMap.second; ++it)
                    {
                        if (it->second == newRecord)
                        {
                            TtoVaultRecordMap->erase(it);
                            break;
                        }
                    }
                }
            );

            VaultRecordSorters.emplace(keyName, [=](std::function<bool(const VaultRecordRef&)> functionToSortedData, bool isReverse)
            {
                if (!isReverse)
                {
                    for (const auto& it : *TtoVaultRecordMap)
                        if(!functionToSortedData(VaultRecordRef(it.second, &VaultHashMapStructure, &VaultMapStructure, &RecursiveReadWriteMtx)))
                            break;
                }
                else
                {
                    for (auto it = TtoVaultRecordMap->rbegin(); it != TtoVaultRecordMap->rend(); ++it)
                        if(!functionToSortedData(VaultRecordRef(it->second, &VaultHashMapStructure, &VaultMapStructure, &RecursiveReadWriteMtx)))
                            break;
                }
            });
            
            // Add new data to record set
            for (auto& it : RecordsSet)
            {
                it->SetData(keyName, defaultKeyValue);
                TtoVaultRecordHashMap->emplace(defaultKeyValue, it);
                TtoVaultRecordMap->emplace(defaultKeyValue, it);
            }

            RecursiveReadWriteMtx.WriteUnlock();
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
            \param [in] defaultKeyValue the value of the key

            \return returns true if the key was found otherwise returns false
        */
        template <class T>
        bool GetKeyValue(const std::string& keyName, T& defaultKeyValue) const
        {
            bool res;
            RecursiveReadWriteMtx.ReadLock();
            res = RecordTemplate.GetData(keyName, defaultKeyValue);
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        /// \brief The method for deleting the key
        /// \param [in] keyName the key to remove
        void RemoveKey(const std::string& keyName);

        /// \brief Method to create new VaultRecord. A record will be created by copying RecordTemplate.
        /// \return ref to new record 
        VaultRecordRef CreateRecord();

        /**
            \brief Method to create new VaultRecord.

            The method accepts a vector of pairs, the first element of the pair is a string with a key, 
            and the second element of the pair are key values of any type.
            The order of the pairs is not important, assignment takes place by key. The number of pairs can be any, 
            for all keys for which no value has been specified, the default value will remain

            The Vault in the example has 2 keys. One is the id of the int type, and the second is the name of the std::string type
            Usage example:

            \code{.cpp}
                // A record with id 0 and name mrognor will be created
                vlt.CreateRecord({ {"id", 0}, {"name", std::string("mrognor")} });
            \endcode

            or

            \code{.cpp}
                // A record with id 0 and name mrognor will be created
                vlt.CreateRecord({ {"name", std::string("mrognor")}, {"id", 0} });
            \endcode

            or

            \code{.cpp}
                // A record with name mrognor will be created. The Id will be set to the default value
                vlt.CreateRecord({ {"name", std::string("mrognor")} });
            \endcode

            or

            \code{.cpp}
                // A record with id 0 and name mrognor will be created
                std::vector<std::pair<std::string, DataSaver>> params = { {"id", 0}, {"name", std::string("mrognor")} };
                VaultRecordRef vltrr = vlt.CreateRecord(params);
            \endcode

            what is equivalent to such a code without passing values to a function

            \code{.cpp}
                VaultRecordRef vltrr = vlt.CreateRecord();

                vltrr.SetData("id", 0);
                vltrr.SetData<std::string>("name", "mrognor");
            \endcode

            \param [in] params a vector of pairs with data to be put in the Vault

            \return ref to new record 
        */
        VaultRecordRef CreateRecord(const std::vector<std::pair<std::string, DataSaver>>& params);

        /**
            \brief The method for getting a reference to the data inside Vault

            \tparam <T> Any type of data except for c arrays

            \param [in] keyName the name of the key to search for
            \param [in] keyValue the value of the key to be found

            \return ref to requested record 
        */
        template <class T>
        VaultRecordRef GetRecord(const std::string& keyName, const T& keyValue) const
        {
            // Pointer to store map inside VaultStructureHashMap
            std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

            RecursiveReadWriteMtx.ReadLock();
            // Checking whether such a key exists
            if (VaultHashMapStructure.GetData(keyName, TtoVaultRecordHashMap))
            {
                // Iterator to element with T type and keyValue value
                auto TtoVaultRecordIt = TtoVaultRecordHashMap->find(keyValue);
                if (TtoVaultRecordIt != TtoVaultRecordHashMap->end())
                {
                    // Set data to VaultRecordRef
                    VaultRecordRef res(TtoVaultRecordIt->second, &VaultHashMapStructure, &VaultMapStructure, &RecursiveReadWriteMtx);
                    RecursiveReadWriteMtx.ReadUnlock();
                    return res;
                }
            }

            VaultRecordRef res(nullptr, &VaultHashMapStructure, &VaultMapStructure, &RecursiveReadWriteMtx);
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        /**
            \brief The method for getting a vector of references to the data inside Vault

            \tparam <T> Any type of data except for c arrays

            \param [in] keyName the name of the key to search for
            \param [in] keyValue the value of the key to be found

            \return vector with records refs
        */
        template <class T>
        std::vector<VaultRecordRef> GetRecords(const std::string& keyName, const T& keyValue) const
        {
            std::vector<VaultRecordRef> res;
            // Pointer to store map inside VaultStructureHashMap
            std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

            RecursiveReadWriteMtx.ReadLock();
            // Checking whether such a key exists
            if (VaultHashMapStructure.GetData(keyName, TtoVaultRecordHashMap))
            {
                auto begAndEndPair = TtoVaultRecordHashMap->equal_range(keyValue);
                for (auto it = begAndEndPair.first; it != begAndEndPair.second; ++it)
                    res.emplace_back(VaultRecordRef(it->second, &VaultHashMapStructure, &VaultMapStructure, &RecursiveReadWriteMtx));
            }

            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        /// \brief A method for deleting all data and keys
        void DropVault();

        /// \brief A method for deleting all data, but keeping all keys
        void DropData();

        /// \brief Method for deleting a record from a Vault
        /// \param recordRefToErase the reference to the record that needs to be deleted
        void EraseRecord(const VaultRecordRef& recordRefToErase);

        /// \brief Method for getting the number of records
        /// \return number of records
        std::size_t Size() const;

        /// \brief The method for getting all the keys
        /// \return vector with keys
        std::vector<std::string> GetKeys() const;

        /**
            \brief Method for getting sorted records

            \param [in] keyName The key by which the data should be sorted
            \param [in] isReverse Sort in descending order or descending order. By default, ascending
            \param [in] amountOfRecords The number of records. By default, everything is

            \return A vector with links to records. The order of entries in the vector is determined by the amountOfRecords parameter
        */
        std::vector<VaultRecordRef> GetSortedRecords(const std::string& keyName, const bool& isReverse = false, const std::size_t& amountOfRecords = -1) const;

        /**
            \brief Method for handle sorted records

            \tparam <T> A function that takes const VaultRecordRef& as a parameter and returns bool.
            
            \param [in] keyName The key by which the data should be sorted
            \param [in] func A function takes const VaultRecordRef& as a parameter. If you need the function to be called again for the next record, 
            then this function call should return true. To stop the loop and interrupt the processing of sorted data, the function must return false. 
            To get values from a function, use lambdas and context capture
            \param [in] isReverse Sort in descending order or descending order. By default, ascending
            \param [in] amountOfRecords The number of records. By default, everything is

            The function iterate over all records sorted by the keyName parameter, in the order specified by the isReverse parameter. 
            For each record, the function passed in the func parameter is called.
            This function does not sort the data when it is called, the sorted data is already stored inside the Vault.
        */
        template<class F>
        void SortBy(const std::string& keyName, const F&& func, const bool& isReverse = false, const std::size_t& amountOfRecords = -1) const
        {
            std::size_t counter = 0;

            RecursiveReadWriteMtx.ReadLock();
            
            /// \todo Проверка
            VaultRecordSorters.find(keyName)->second([&](const VaultRecordRef& vaultRecordRef)
                {
                    if (counter >= amountOfRecords) return false;
                    
                    func(vaultRecordRef);
                    ++counter;
                    return true;
                }, isReverse);

            RecursiveReadWriteMtx.ReadUnlock();
        }

        /// \brief A method for displaying the contents of a Vault on the screen
        /// \param [in] amountOfRecords The number of records to be printed. The default value is -1, which means that all entries will be output
        void PrintVault(const std::size_t& amountOfRecords = -1) const;
        
        /// \brief A method for displaying the contents of a Vault as a table on the screen
        /// \param [in] amountOfRecords The number of records to be printed. The default value is -1, which means that all entries will be output
        /// \param [in] keys vector of keys to be printed. By default, the vector is empty, which means that all keys will be output
        void PrintAsTable(const std::size_t& amountOfRecords = -1, const std::vector<std::string> keys = {}) const;

        /// \brief Default destructor
        ~Vault();
    };
}