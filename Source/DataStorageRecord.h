#pragma once

#include <vector>
#include <sstream>
#include <mutex>

#include "DataStorageClasses.h"
#include "DataContainer.h"
#include "ReadWriteMutex.h"

// Class declaration
class DataStorageRecordRef;

/**
    \brief A class for storing data inside DataStorage

    It is a wrapper over the Data Hash Map, but adds a functionality to invalidate DataStorageRecordRef's pointing to an object of this class.  
    The functionality from HashMap stores allows you to store data of any type and provide access to them using string keys.  
*/ 
class DataStorageRecord : public DataHashMap
{
private:
    // Variable to store object validity
    bool IsValid = true;
    // Validity counter
    std::size_t RefCounter = 0;
    // Mutex for thread safety
    std::mutex* Mtx = nullptr;
public:
    /// \brief Default constructor
    DataStorageRecord();

    /// \brief Copy constructor
    DataStorageRecord(const DataStorageRecord& other);

    /// \brief A method for increasing the number of references to an object
    void AddRef();

    /// \brief A method to reduce the number of references to an object
    void RemoveRef();

    /// \brief A method to indicate that the record is no longer valid and is not inside DataStorage
    void Invalidate();

    /// \brief A method for checking whether a record is inside a DataStorage
    /// \return returns the validity of the record
    bool GetIsValid();

    /// \brief Default destructor
    ~DataStorageRecord();
};

/**
    \brief A class that provides access to data inside DataStorage.

    This class works as a reference to the data inside the DataStorage, you can use it to change the data inside the DataStorage.
    If two objects of the class point to the same DataStorageRecord inside the DataStorage, 
    then when the data in one object changes, the data in the other object will also change.
    If the record pointed to by an object of this class is deleted, the isValid function will return false, 
    and any attempt to access or change the data will be ignored.
*/
class DataStorageRecordRef
{
private:
    // Pointer to DataStorageRecord inside DataStorage
    DataStorageRecord* DataRecord = nullptr;

    // Pointer to DataStorageStructureHashMap 
    DataStorageStructureHashMap* DataStorageHashMapStructure = nullptr;

    // Pointer to DataStorageStructureMap 
    DataStorageStructureMap* DataStorageMapStructure = nullptr;

    // Pointer to recursive read write mutex inside data storage
    RecursiveReadWriteMutex* DataStorageRecucrsiveReadWriteMtx = nullptr;
public:

    /// Making the DataStorage class friendly so that it has access to the internal members of the DataStorageRecordRef class
    friend DataStorage;

    /// Making the std::hash<DataStorageRecordRef> struct friendly so that it has access to the internal members of the DataStorageRecordRef class
    friend std::hash<DataStorageRecordRef>;

    /**
        \brief Constructor
        
        \param [in] dataStorageRecord A pointer to the DataStorage referenced by the class object DataStorageRecordRef
        \param [in] dataStorageStructureHashMap The internal structure of the DataStorage, represented by a hash table
        \param [in] dataStorageStructureMap The internal structure of the DataStorage, represented by a binary tree
        \param [in] dataStorageRecucrsiveReadWriteMtx Pointer to DataStorage mutex used for thread safety
    */
    DataStorageRecordRef(DataStorageRecord* dataStorageRecord, 
        DataStorageStructureHashMap* dataStorageStructureHashMap, 
        DataStorageStructureMap* dataStorageStructureMap,
        RecursiveReadWriteMutex* dataStorageRecucrsiveReadWriteMtx);

    /// \brief Copy constructor
    /// \param [in] other other DataStorageRecordRef object
    DataStorageRecordRef(const DataStorageRecordRef& other);

    DataStorageRecordRef& operator=(const DataStorageRecordRef& other);
    
    /// \brief Comparison operator
    /// \param [in] other the object to compare with
    /// \return true if the objects are equal, otherwise false
    bool operator==(const DataStorageRecordRef& other) const;

    /// \brief A method for obtaining a unique record identifier
    ///  Important. Two DataStorageRecordRef objects pointing to the same record will return the same value
    /// \return the unique identifier of the record
    std::string GetRecordUniqueId() const;

    /**
        \brief Method for updating data inside DataStorage

        \tparam <T> Any type of data except for c arrays

        Using this method, you can change the values inside the DataStorageRecord inside the DataStorage

        \param [in] key the key whose value needs to be changed
        \param [in] data new key data value

        \return returns true if the key was found otherwise returns false
    */
    template <class T>
    bool SetData(const std::string& key, const T& data)
    {
        // A pointer for storing a std::unordered_multimap in which a template data type is used as a key, 
        // and a pointer to the DataHashMap is used as a value
        std::unordered_multimap<T, DataStorageRecord*>* TtoDataStorageRecordHashMap = nullptr;

        // A pointer for storing a std::multimap in which a template data type is used as a key, 
        // and a pointer to the DataHashMap is used as a value
        std::multimap<T, DataStorageRecord*>* TtoDataStorageRecordMap = nullptr;

        // Lock DataStorage to write
        DataStorageRecucrsiveReadWriteMtx->WriteLock();

        // Get std::unordered_multimap with T key and DataStorageRecord* value
        if (!DataStorageHashMapStructure->GetData(key, TtoDataStorageRecordHashMap)) 
        {
            DataStorageRecucrsiveReadWriteMtx->WriteUnlock();
            return false;
        }

        // Get std::multimap with T key and DataStorageRecord* value
        if (!DataStorageMapStructure->GetData(key, TtoDataStorageRecordMap))
        {
            DataStorageRecucrsiveReadWriteMtx->WriteUnlock();
            return false;
        }

        if (DataRecord == nullptr || !DataRecord->GetIsValid())
        {
            DataStorageRecucrsiveReadWriteMtx->WriteUnlock();
            return false;
        }

        // Get the current value of the key key inside the DataStorageRecordRef and save it for further work
        T oldData;
        DataRecord->GetData(key, oldData);

        // Remove oldData from TtoDataStorageRecordHashMap from DataStorageHashMapStructure
        auto FirstAndLastIteratorsWithKeyOnHashMap = TtoDataStorageRecordHashMap->equal_range(oldData);

        // Iterate over all data records with oldData key
        for (auto& it = FirstAndLastIteratorsWithKeyOnHashMap.first; it != FirstAndLastIteratorsWithKeyOnHashMap.second; ++it)
        {
            // Find required data record
            if (it->second == DataRecord)
            {
                TtoDataStorageRecordHashMap->erase(it);
                break;
            }
        }

        // Add new data to TtoDataStorageRecordHashMap to DataStorage DataStorageHashMapStructure
        TtoDataStorageRecordHashMap->emplace(data, DataRecord);

        // Remove oldData from TtoDataStorageRecordMap from DataStorageMapStructure
        auto FirstAndLastIteratorsWithKeyOnMap = TtoDataStorageRecordMap->equal_range(oldData);

        // Iterate over all data records with oldData key
        for (auto& it = FirstAndLastIteratorsWithKeyOnMap.first; it != FirstAndLastIteratorsWithKeyOnMap.second; ++it)
        {
            // Find required data record
            if (it->second == DataRecord)
            {
                TtoDataStorageRecordMap->erase(it);
                break;
            }
        }

        // Add new data to TtoDataStorageRecordMap to DataStorage DataStorageMapStructure
        TtoDataStorageRecordMap->emplace(data, DataRecord);

        // Update data inside DataStorageRecord pointer inside DataStorageRecordRef and DataStorage
        DataRecord->SetData(key, data);

        // Unlock DataStorage
        DataStorageRecucrsiveReadWriteMtx->WriteUnlock();
        return true;
    }

    /**
        \brief Method for updating data inside DataStorage

        Using this method, you can change the values inside the DataStorageRecord inside the DataStorage.  
        See DataStorage::CreateRecord(std::vector<std::pair<std::string, DataSaver>> params) for more information

        \param [in] params a vector of pairs with data to be put in the DataStorage
    */
    void SetData(const std::vector<std::pair<std::string, DataSaver>>& params);

    /**
        \brief A method for getting data using a key

        \tparam <T> Any type of data except for c arrays
        
        \param [in] key the key whose value should be obtained
        \param [in] data reference to record the received data

        \return returns true if the data was received, otherwise false
    */
    template <class T>
    bool GetData(const std::string& key, T& data) const
    {
        bool res;
        DataStorageRecucrsiveReadWriteMtx->ReadLock();
        if (DataRecord->GetIsValid()) res = DataRecord->GetData(key, data);
        DataStorageRecucrsiveReadWriteMtx->ReadUnlock();
        return res;
    }

    /// \brief A function to check the validity of a class object
    /// An object may no longer be valid if the record it refers to has been deleted
    /// \return returns true if the object is valid, otherwise false
    bool IsValid() const;

    /// \brief A method for displaying DataStorageRecordRef content on the screen
    void PrintRecord() const;
    
    /// A method for decoupling a class object from record
    /// \todo Нужен ли этот метод
    void Unlink();

    /// \brief Default destructor
    ~DataStorageRecordRef();
};
