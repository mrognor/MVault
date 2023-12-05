#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <typeinfo>
#include <functional>
#include <vector>

#include "DataSaver.h"
#include "SmartPointerWrapper.h"
#include "DataContainer.h"
#include "DataStorageRecord.h"
#include "DataStorageRecordSet.h"

// A class for storing data with the ability to quickly access data using a variety of different keys
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

    // Unordered_map of functions that add a new element to the DataStorageStruct
    std::unordered_map<std::string, std::function<void(DataStorageRecord*)>> DataStorageRecordAdders;


    std::unordered_map<std::string, std::function<void()>> DataStorageRecordCleaners;


    std::unordered_map<std::string, std::function<void(DataStorageRecord* newRecord)>> DataStorageRecordErasers;

    // Unordered set with all DataStorageRecord pointers
    std::unordered_set<DataStorageRecord*> RecordsSet;
public:

    DataStorage() {};

    DataStorage& operator= (const DataStorage& other) = delete;

    DataStorage(const DataStorage& other) = delete;

    // Template function to add new key with default value to DataStorage
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

        DataStorageRecordCleaners.emplace(keyName, [=]()
            {
                TtoDataStorageRecordMap->clear();
            }
        );

        DataStorageRecordErasers.emplace(keyName, [=](DataStorageRecord* newRecord)
            {
                T recordTData;
                newRecord->GetData(keyName, recordTData);
                auto FirstAndLastIteratorsWithKey = TtoDataStorageRecordMap->equal_range(recordTData);

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

        for (auto& it : RecordsSet)
        {
            it->AddData(keyName, defaultKeyValue);
            TtoDataStorageRecordMap->emplace(defaultKeyValue, it);
        }
    }

    void RemoveKey(const std::string& keyName)
    {
        RecordTemplate.EraseData(keyName);
        DataStorageStructure.EraseData(keyName);
        DataStorageRecordAdders.erase(keyName);
        DataStorageRecordCleaners.erase(keyName);
        DataStorageRecordErasers.erase(keyName);

        for (auto& it : RecordsSet)
            it->EraseData(keyName);
    }

    // Method to create new DataStorageRecord. A record will be created by copying RecordTemplate
    DataStorageRecordRef CreateNewRecord()
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

    // Method to get reference to data inside DataStorage
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
                foundedRecord.SetDataStorageRecordPtr(TtoDataStorageRecordIt->second);
                foundedRecord.SetDataStorageStructPtr(&DataStorageStructure);
                return true;
            }
        }

        return false;
    }

    // Method to get reference to data inside DataStorage
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

    void DropDataStorage()
    {
        // Clear record template
        RecordTemplate.Clear();

        // Delete all unordered maps inside DataStorageStructure
        for (auto& it : DataStorageStructure)
            it.second.ResetData();

        // Clear DataStorageStructure
        DataStorageStructure.Clear();

        DataStorageRecordAdders.clear();
        DataStorageRecordCleaners.clear();
        DataStorageRecordErasers.clear();

        // Delete all Records
        for (auto& it : RecordsSet)
            delete it;

        // Clear RecordsSet
        RecordsSet.clear();
    }

    void DropData()
    {
        // Call functions to clear DataStorageStructure without
        for (auto& it : DataStorageRecordCleaners)
            it.second();

        // Delete all Records
        for (auto& it : RecordsSet)
            delete it;

        // Clear RecordsSet
        RecordsSet.clear();
    }

    void EraseRecord(const DataStorageRecordRef& recordRefToErase)
    {
        DataStorageRecord* tmpRec = recordRefToErase.GetRawData();
        for (auto& erasers : DataStorageRecordErasers)
            erasers.second(tmpRec);

        RecordsSet.erase(tmpRec);
        delete tmpRec;
    }

    void EraseRecords(const DataStorageRecordSet& recordsToErase)
    {
        for (auto& recordRef : recordsToErase)
        {
            DataStorageRecord* tmpRec = recordRef.GetRawData();
            for (auto& erasers : DataStorageRecordErasers)
                erasers.second(tmpRec);

            RecordsSet.erase(tmpRec);
            delete tmpRec;
        }
    }

    std::size_t Size()
    {
        return RecordsSet.size();
    }

    ~DataStorage()
    {
        // Clear DataStorageStructure
        for (auto& it : DataStorageStructure)
            it.second.ResetData();

        // Clear all records
        for (auto& it : RecordsSet)
            delete it;
    }
};

int main()
{
    // Known limitations: data saver cannot store arrays

    std::cout << "Phase 1. Simple demo" << std::endl;

    DataStorage ds;
    ds.AddKey("id", -1);
    ds.AddKey<std::string>("name", "none");

    DataStorageRecordRef dsrr = ds.CreateNewRecord();

    if (ds.GetRecord<int>("id", -1, dsrr))
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "1: " << res << std::endl;
    }

    dsrr.SetData("id", 0);
    dsrr.SetData<std::string>("name", "mrognor");

    if (ds.GetRecord<int>("id", -1, dsrr))
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "2: " << res << std::endl;
    }

    if (ds.GetRecord<int>("id", 0, dsrr))
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "3: " << res << std::endl;
    }

    if (ds.GetRecord<std::string>("name", "mrognor", dsrr))
    {
        int res;
        if (dsrr.GetData("id", res))
            std::cout << "4: " << res << std::endl;
    }

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 1);
    dsrr.SetData<std::string>("name", "moop");

    if (ds.GetRecord<int>("id", 1, dsrr))
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "5: " << res << std::endl;
    }

    if (ds.GetRecord<std::string>("name", "moop", dsrr))
    {
        int res;
        if (dsrr.GetData("id", res))
            std::cout << "6: " << res << std::endl;
    }

    dsrr.SetData("id", 2);
    if (ds.GetRecord<std::string>("name", "moop", dsrr))
    {
        int res;
        if (dsrr.GetData("id", res))
            std::cout << "7: " << res << std::endl;
    }
    dsrr.SetData("id", 1);

    std::cout << "Phase 2. Runtime key addiction" << std::endl;

    ds.AddKey("gender", true);

    if (ds.GetRecord<bool>("gender", true, dsrr))
    {
        bool res;
        if (dsrr.GetData("gender", res))
        {
            if (res) std::cout << "1: true" << std::endl;
            else std::cout << "1: false" << std::endl;
        }
    }

    std::cout << "Phase 3. Runtime key remove" << std::endl;

    ds.RemoveKey("name");

    if (ds.GetRecord<std::string>("name", "moop", dsrr))
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "1: " << res << std::endl;
    }
    else
        std::cout << "1: No data with name key" << std::endl;

    std::cout << "Phase 4. Getting set of records using key" << std::endl;

    ds.GetRecord("id", 0, dsrr);
    dsrr.SetData("gender", false);

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 2);

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 3);
    dsrr.SetData("gender", false);

    DataStorageRecordSet dsrs;
    ds.GetRecords("gender", true, dsrs);

    for (auto it : dsrs)
    {
        int res;
        if (it.GetData("id", res))
            std::cout << "True gender: " << res << std::endl;
    }

    ds.GetRecords("gender", false, dsrs);

    for (auto it : dsrs)
    {
        int res;
        if (it.GetData("id", res))
            std::cout << "False gender: " << res << std::endl;
    }

    std::cout << "Phase 5. Clear data storage" << std::endl;

    ds.DropData();

    ds.GetRecords("gender", false, dsrs);
    std::cout << "False gender amount: " << dsrs.Size() << std::endl;

    std::cout << "Phase 5. Sets: union, except, intersection, alterintersection" << std::endl;

    ds.DropDataStorage();

    DataStorageRecordSet opset, set0, set1, set2, set3;

    ds.AddKey("id", -1);
    ds.AddKey("type", -1);

    for (int i = 0; i < 20; ++i)
    {
        dsrr = ds.CreateNewRecord();
        dsrr.SetData("id", i);
        dsrr.SetData("type", i % 4);
    }

    ds.GetRecords("type", 0, set0);
    std::cout << "Set0: ";
    for (auto it : set0)
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    ds.GetRecords("type", 1, set1);
    std::cout << "Set1: ";
    for (auto it : set1)
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    ds.GetRecords("type", 2, set2);
    std::cout << "Set2: ";
    for (auto it : set2)
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    ds.GetRecords("type", 3, set3);
    std::cout << "Set3: ";
    for (auto it : set3)
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "Union02: ";
    for (auto it : Union(set0, set2))
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "Union03: ";
    for (auto it : Union(set0, set3))
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "Intersection Union01 Union12: ";
    for (auto it : Intersection(Union(set0, set1), Union(set1, set2)))
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "AlterIntersection Union01 Union12: ";
    for (auto it : AlterIntersection(Union(set0, set1), Union(set1, set2)))
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "Except 0 Union01: ";
    for (auto it : Except(Union(set0, set1), set0))
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "Phase 6. Data erasing" << std::endl;

    ds.DropDataStorage();

    ds.AddKey("id", -1);
    ds.AddKey("type", -1);

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 0);
    dsrr.SetData("type", 0);

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 1);
    dsrr.SetData("type", 0);

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 1);
    dsrr.SetData("type", 0);

    std::cout << "Size before removing: " << ds.Size() << std::endl;

    ds.EraseRecord(dsrr);

    std::cout << "Size after removing one last element: " << ds.Size() << std::endl;

    ds.GetRecords("type", 0, dsrs);
    
    ds.EraseRecords(dsrs);
    std::cout << "Size after removing set: " << ds.Size() << std::endl;

    std::cout << "Phase 7. RecordRef invalidation" << std::endl;

    ds.DropDataStorage();

    ds.AddKey("id", -1);
    
    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 0);
    ds.EraseRecord(dsrr);

    std::cout << dsrr.IsValid() << std::endl;
}