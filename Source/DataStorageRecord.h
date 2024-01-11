#pragma once

#include <vector>
#include <sstream>
#include <mutex>

#include "DataStorageClasses.h"
#include "DataContainer.h"
#include "SmartPointerWrapper.h"
#include "ReadWriteMutex.h"

// Class declaration
class DataStorageRecordRef;

/**
    \brief A class for storing data inside DataStorage

    It is a wrapper over the Data Hash Map, but adds a SmartPointerWrapper to invalidate DataStorageRecordRef's pointing to an object of this class.  
    The functionality from HashMap stores allows you to store data of any type and provide access to them using string keys.  
    IsDataStorageRecordValid is created when the class object is created and deleted when the object is deleted.
    All DataStorageRecordRef's pointing to an object of this class store copies of IsDataStorageRecordValid inside themselves. 
    Thus, when deleting IsDataStorageRecordValid, all DataStorageRecordRef's pointing to this object learn about its destruction and cease to be valid
*/ 
class DataStorageRecord : public DataHashMap
{
private:
    // A variable for invalidating DataStorageRecordRef's pointing to this object. Before destroying the object, the value will be set to false.
    SmartPointerWrapper<bool> IsDataStorageRecordValid;

    // Smart pointer wrapper to create mutex at DataStorageRecord creation and deletion this mutex only after destroying last reference to this record and record itself
    SmartPointerWrapper<RecursiveReadWriteMutex> DataStorageRecordRecursiveReadWriteMutex;
public:
    
    /// Declaring the DataStorageRecordRef class to access its private members
    friend DataStorageRecordRef;
    
    /// Declaring the DataStorage class to access its private members
    friend DataStorage;

    /// Default constructor
    DataStorageRecord();

    /// \brief Copy constructor
    /// \param [in] recordTemplate object to be copied. Usually it is record template from DataStorage
    DataStorageRecord(const DataStorageRecord& recordTemplate);

    /// Default destructor
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

    // Smart pointer wrapper to get info about data storage record validity
    SmartPointerWrapper<bool> IsDataStorageRecordValid;

    // Smart pointer wrapper to create mutex at DataStorageRecord creation and deletion this mutex only after destroying last reference to this record and record itself
    SmartPointerWrapper<RecursiveReadWriteMutex> DataStorageRecordRecursiveReadWriteMutex;
public:

    /// Making the DataStorage class friendly so that it has access to the internal members of the DataStorageRecordRef class
    friend DataStorage;

    /// Making the std::hash<DataStorageRecordRef> struct friendly so that it has access to the internal members of the DataStorageRecordRef class
    friend std::hash<DataStorageRecordRef>;

    /**
        \brief Constructor
        \todo Добавить описание
    */
    DataStorageRecordRef(DataStorageRecord* dataStorageRecord, 
        DataStorageStructureHashMap* dataStorageStructureHashMap, 
        DataStorageStructureMap* dataStorageStructureMap,
        RecursiveReadWriteMutex* dataStorageRecucrsiveReadWriteMtx);

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

        // Check DataStorageRecordValidity
        if (!*IsDataStorageRecordValid.GetData())
        {
            DataStorageRecucrsiveReadWriteMtx->WriteUnlock();
            return false;
        }

        // Lock DataStorageRecord to write
        DataStorageRecordRecursiveReadWriteMutex.GetData()->WriteLock();

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

        // Unlock DataStorageRecord
        DataStorageRecordRecursiveReadWriteMutex.GetData()->WriteUnlock();
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
        DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadLock();
        res = DataRecord->GetData(key, data);
        DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadUnlock();
        return res;
    }

    /// \brief A function to check the validity of a class object
    /// An object may no longer be valid if the record it refers to has been deleted
    /// \return returns true if the object is valid, otherwise false
    bool IsValid() const;

    /// A method for decoupling a class object from record
    /// \todo Нужен ли этот метод
    void Unlink();
};

/// Specialization of the hashing function from std to use the class DataStorageRecordRef inside std containers like std::unordered_map or std::set.
template <>
struct std::hash<DataStorageRecordRef>
{
    std::size_t operator()(const DataStorageRecordRef& k) const
    {
        std::size_t res;
        k.DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadLock();
        res = std::hash<DataStorageRecord*>()(k.DataRecord);
        k.DataStorageRecordRecursiveReadWriteMutex.GetData()->ReadUnlock();
        return res;
    }
};
