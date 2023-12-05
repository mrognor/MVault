#pragma once

#include "DataContainer.h"
#include "SmartPointerWrapper.h"

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
public:
    
    /// Declaring the DataStorageRecordRef class to access its private members
    friend DataStorageRecordRef;

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
    // Pointer to DataStorageStruct 
    DataStorageStruct* DataStorageStructure = nullptr;
    // Smart pointer wrapper to get info about data storage record validity
    SmartPointerWrapper<bool> IsDataStorageRecordValid;
public:

    /// Default constructor 
    DataStorageRecordRef();

    /// \brief Constructor
    /// \param [in] data a pointer to the record that will be stored inside DataStorageRecordRef
    /// \param [in] dataStorageStructure pointer to the DataStorage structure
    DataStorageRecordRef(DataStorageRecord* data, DataStorageStruct* dataStorageStructure);

    /// \todo Method to replace with friend
    void SetDataStorageRecordPtr(DataStorageRecord* data);
    /// \todo Method to replace with friend
    void SetDataStorageStructPtr(DataStorageStruct* dataStorageStructure);

    /// \brief Comparison operator
    /// \param [in] other the object to compare with
    /// \return true if the objects are equal, otherwise false
    bool operator==(const DataStorageRecordRef& other) const;

    /// \todo Method to replace with friend
    DataStorageRecord* GetRawData() const;

    /**
        \brief Method for updating data inside DataStorage

        Using this method, you can change the values inside the DataStorageRecord inside the DataStorage

        \param [in] key the key whose value needs to be changed
        \param [in] data new key data value
    */
    template <class T>
    void SetData(const std::string& key, const T& data)
    {
        // A pointer for storing a std::unordered_multimap in which a template data type is used as a key, 
        // and a pointer to the DataHashMap is used as a value
        std::unordered_multimap<T, DataStorageRecord*>* TtoDataStorageRecordMap = nullptr;
        DataStorageStructure->GetData(key, TtoDataStorageRecordMap);

        // Get the current value of the key key inside the DataStorageRecordRef and save it for further work
        T oldData;
        DataRecord->GetData(key, oldData);

        // Remove oldData from TtoDataStorageRecordMap to DataStorage DataStorageStructure
        auto FirstAndLastIteratorsWithKey = TtoDataStorageRecordMap->equal_range(oldData);

        // Iterate over all data records with oldData key
        for (auto& it = FirstAndLastIteratorsWithKey.first; it != FirstAndLastIteratorsWithKey.second; ++it)
        {
            // Find required data record
            if (it->second == DataRecord)
            {
                TtoDataStorageRecordMap->erase(it);
                break;
            }
        }

        // Add new data to TtoDataStorageRecordMap to DataStorage DataStorageStructure
        TtoDataStorageRecordMap->emplace(data, DataRecord);

        // Update data inside DataStorageRecord pointer inside DataStorageRecordRef and DataStorage
        DataRecord->SetData(key, data);
    }

    /**
        \brief A method for getting data using a key

        \param [in] key the key whose value should be obtained
        \param [in] data reference to record the received data

        \return returns true if the data was received, otherwise false
    */
    template <class T>
    bool GetData(const std::string& key, T& data)
    {
        return DataRecord->GetData(key, data);
    }

    /// \brief A function to check the validity of a class object
    /// An object may no longer be valid if the record it refers to has been deleted
    /// \return returns true if the object is valid, otherwise false
    bool IsValid();
};

/// Specialization of the hashing function from std to use the class DataStorageRecordRef inside std containers like std::unordered_map or std::set.
template <>
struct std::hash<DataStorageRecordRef>
{
    std::size_t operator()(const DataStorageRecordRef& k) const
    {
        return std::hash<DataStorageRecord*>()(k.GetRawData());
    }
};
