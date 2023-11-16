#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <typeinfo>
#include <functional>
#include <vector>

// Class to make real time type check
class DataTypeSaver
{
private:
    // Type info struct to save type
    const std::type_info& TypeInfo;

public:
    // Constructor to set type variable
    DataTypeSaver(const std::type_info& type) : TypeInfo(type) {}

    // Type getter
    const std::type_info& GetDataType() { return TypeInfo; }
};

// Class to store data with any type
// If pointers are stored in the class object, then you can set a custom data deletion function. 
class DataSaver
{
private:
    // Void pointer to save pointer to any data
    void* Ptr = nullptr;
    
    // Pointer to data type saver
    DataTypeSaver* DataType = nullptr;
    
    // Variable to store data type size
    std::size_t DataTypeSize = 0;
    
    // Pointer to copy function. Required to DataSaver copy
    void (*CopyFunc)(void*& dst, const void* src) = nullptr;

    // Pointer to copy function. Required to delete data if it is pointer
    void (*DeleteFunc)(const void* ptr) = nullptr;

public:
    // Default constructor
    DataSaver() {}

    // Copy constructor
    DataSaver(const DataSaver& dataSaver)
    {
        // Call operator= method
        *this = dataSaver;
    }

    // Operator= definition to copy data from DataSaver.
    DataSaver& operator=(const DataSaver& dataSaver)
    {
        // We check that there is no self-bonding
        if (&dataSaver != this)
        {
            // Clear Ptr if it was data before
            if (Ptr != nullptr)
                free(Ptr);

            // Clear DataType if it was data before
            if (DataType != nullptr)
                delete DataType;

            // Check that dataSaver is not empty
            if (dataSaver.Ptr != nullptr)
            {
                // Create new DataType object copying data type from dataSaver
                DataType = new DataTypeSaver(dataSaver.DataType->GetDataType());
                // Copying data type size from dataSaver
                DataTypeSize = dataSaver.DataTypeSize;
                // Set new CopyFunc from dataSaver
                CopyFunc = dataSaver.CopyFunc;
                // Call new copy func to copy data from dataSaver void pointer to local void pointer. Data to Ptr will be allocated inside function
                CopyFunc(Ptr, dataSaver.Ptr);
                // Set delete function from dataSaver
                DeleteFunc = dataSaver.DeleteFunc;
            }
            else
            {
                // If dataSaver is empty clear the variables
                Ptr = nullptr;
                DataType = nullptr;
            }
        }

        return *this;
    }

    // Template constructor to save data inside DataSaver
    template<class T>
    DataSaver(const T& data)
    {
        SetData(data);
    }

    // Template constructor to save data and delete function inside DataSaver
    template<class T, class F>
    DataSaver(const T& data, F&& deleteFunc)
    {
        SetData(data, deleteFunc);
    }

    // Template method to save data inside DataSaver
    template <class T>
    void SetData(const T& data)
    {
        SetData(data, nullptr);
    }

    // Template method to save data and delete function inside DataSaver
    template <class T, class F>
    void SetData(const T& data, F&& deleteFunc)
    {
        // Clear Ptr if it was data before
        if (Ptr != nullptr)
            free(Ptr);

        // Clear DataType if it was data before
        if (DataType != nullptr)
            delete DataType;

        // Create new T type object and save it pointer like void ptr. Data from data will be copying using copy constructor
        Ptr = (void*)new T(data);

        // Create new DataType object to save data type
        DataType = new DataTypeSaver(typeid(data));
        // Save T size
        DataTypeSize = sizeof(T);

        // Set new CopyFunc. It is get to void pointers and convert void pointers to T pointers and copy data.
        CopyFunc = [](void*& dst, const void* src)
        {
            // Convert src pointer to T pointer and get data from T pointer.
            // Use T copy constructor to create T object.
            // Allocate new memory to T type and convert it to void pointer.
            dst = (void*) new T(*(T*)src);
        };

        // Set delete function from dataSaver
        DeleteFunc = deleteFunc;
    }

    // Template method to get data from DataSaver. Return true if it was data inside.
    template <class T>
    bool GetData(T& data)
    {
        // Check data type stored in DataSaver
        if (DataType != nullptr && DataType->GetDataType() != typeid(data))
        {
            std::cout << "Wrong type! Was: " + std::string(DataType->GetDataType().name()) + " Requested: " + typeid(data).name() << std::endl;
            return false;
        }

        // Check that was data inside Ptr
        if (Ptr == nullptr)
            return false;

        // Copy data from Ptr to data
        data = *(T*)Ptr;
        return true;
    }

    // Call DeleteFunc to custom data deletion. If the data deletion function has not been installed, then this method will not do anything
    void DeleteData()
    {
        if (DeleteFunc != nullptr)
        {
            DeleteFunc(Ptr);
            DeleteFunc = nullptr;
        }
    }

    // Destructor
    ~DataSaver()
    {
        if (Ptr != nullptr)
            free(Ptr);

        if (DataType != nullptr)
            delete DataType;
    }
};

// A container class for storing data of any type. Allows to specify required container.
// Container have to support find(), begin(), end(), emplace(), erase()
// If a pointer is written to one of the elements, then you can set a custom data deletion function. 
template <class C>
class DataContainer
{
protected:
    // Hash map to store all DataSaver's
    C Data;
    
public:
    // Iterators
    typedef typename C::iterator iterator;
    typedef typename C::const_iterator const_iterator;

    // Begin and end functions to work with iterators and foreach loop
    inline iterator begin() noexcept { return Data.begin(); }
    inline const_iterator cbegin() const noexcept { return Data.cbegin(); }
    inline iterator end() noexcept { return Data.end(); }
    inline const_iterator cend() const noexcept { return Data.cend(); }

    // Method to add new element with data inside container
    template <class T>
    void AddData(const std::string& key, const T& data)
    {
        Data.emplace(key, DataSaver(data));
    }

    // Method to add new element with data and custom delete function inside container
    template <class T, class F>
    void AddData(const std::string& key, const T& data, F&& deleteFunc)
    {
        Data.emplace(key, DataSaver(data, deleteFunc));
    }

    // Method to update element data
    template <class T>
    void SetData(const std::string& key, const T& data)
    {
        SetData(key, data, nullptr);
    }

    // Method to update element data and delete function
    template <class T, class F>
    void SetData(const std::string& key, const T& data, F&& deleteFunc)
    {
        auto f = Data.find(key);

        if (f == Data.end())
            AddData(key, data, deleteFunc);
        else
            f->second.SetData(data, deleteFunc);
    }

    // Method to get data from container using key
    template <class T>
    bool GetData(const std::string& key, T& data)
    {
        auto f = Data.find(key);
        if (f == Data.end())
            return false;

        f->second.GetData(data);
        return true;
    }

    // Method for checking the presence of data in the container
    bool IsData(const std::string& key)
    {
        return Data.find(key) != Data.end();
    }

    // Custom data deletion
    void DeleteData(const std::string& key)
    {
        auto f = Data.find(key);
        if (f != Data.end())
        {
            f->second.DeleteData();
            Data.erase(key);
        }
    }
};

// A container class for storing data of any type. 
// As the base class of the container, std::unordered_map is used, which is a hash map
// If a pointer is written to one of the elements, then you can set a custom data deletion function. 
class DataHashMap : public DataContainer<std::unordered_map<std::string, DataSaver>> {};

// A container class for storing data of any type. 
// As the base class of the container, std::unordered_multimap is used, which is a hash map
// If a pointer is written to one of the elements, then you can set a custom data deletion function. 
class DataMultiHashMap : public DataContainer<std::unordered_multimap<std::string, DataSaver>>
{
public:
    // Returns a pair of iterators. The first iterator points to the first element with the same key, and the second to the last element
    // Equals std::unordered_multimap<...>::equal_range
    std::pair<DataContainer::iterator, DataContainer::iterator> GetAllData(const std::string& key)
    {
        return Data.equal_range(key);
    }
};

// A simple typedef for HashMap. It is necessary for a more understandable separation of types.
// Represents the data type of each object stored inside the DataStorage
typedef DataHashMap DataStorageRecord;

/*
    A simple typedef for HashMap. It is necessary for a more understandable separation of types.
    Represents the internal structure of the DataStorage.
    A string with the name of the key is used as the key. All keys are the same as in DataStorage.
    The value stores a pointer to std::unordered_multimap<T, DataStorageRecord*>.
    Such a complex structure is needed to quickly search for each key with any type.
    The key type is same as the DataStorage key value type.
    The value is a pointer to DataStorageRecord.
*/
typedef DataHashMap DataStorageStruct;

// A class that provides access to data inside DataStorage.
// This class works as a reference to the data inside the DataStorage, 
// i.e. when the data inside this class changes, they will also change inside the DataStorage
class DataStorageRecordRef
{
private:
    // Pointer to DataStorageRecord inside DataStorage
    DataStorageRecord* Data = nullptr;
    // Pointer to DataStorageStruct 
    DataStorageStruct* DataStorageStructure = nullptr;
public:
    DataStorageRecordRef() {}
    DataStorageRecordRef(DataStorageRecord* data, DataStorageStruct* dataStorageStructure) : Data(data), DataStorageStructure(dataStorageStructure) {}

    void SetDataStorageRecordPtr(DataStorageRecord* data) { Data = data; }
    void SetDataStorageStructPtr(DataStorageStruct* dataStorageStructure) { DataStorageStructure = dataStorageStructure; }

    bool operator==(const DataStorageRecordRef &other) const
    { 
        return Data == other.Data;
    }

    DataStorageRecord* GetRawData() const
    {
        return Data;
    }

    // Method to update data inside DataStorage
    template <class T>
    void SetData(const std::string& key, const T& data)
    {
        // A pointer for storing a std::unordered_multimap in which a template data type is used as a key, 
        // and a pointer to the DataHashMap is used as a value
        std::unordered_multimap<T, DataStorageRecord*>* TtoDataStorageRecordMap = nullptr;
        DataStorageStructure->GetData(key, TtoDataStorageRecordMap);

        // Get the current value of the key key inside the DataStorageRecordRef and save it for further work
        T oldData;
        Data->GetData(key, oldData);
        
        // Remove oldData from TtoDataStorageRecordMap to DataStorage DataStorageStructure
        auto FirstAndLastIteratorsWithKey = TtoDataStorageRecordMap->equal_range(oldData);

        for (auto& it = FirstAndLastIteratorsWithKey.first; it != FirstAndLastIteratorsWithKey.second; ++it)
        {
            if (it->second == Data)
            {
                TtoDataStorageRecordMap->erase(it);
                break;
            }
        }

        // Add new data to TtoDataStorageRecordMap to DataStorage DataStorageStructure
        TtoDataStorageRecordMap->emplace(data, Data);
        
        // Update data inside DataStorageRecord pointer inside DataStorageRecordRef and DataStorage
        Data->SetData(key, data);
    }

    // Method to get data from container using key
    template <class T>
    bool GetData(const std::string& key, T& data)
    {
        return Data->GetData(key, data);
    }
};

// Specialize hash function to DataStorageRecordRef using DataStorageRecord pointer as value
template <>
struct std::hash<DataStorageRecordRef>
{
    std::size_t operator()(const DataStorageRecordRef& k) const
    {
        return std::hash<DataStorageRecord*>()(k.GetRawData());
    }
};

class DataStorageRecordSet
{
private:
    std::unordered_set<DataStorageRecordRef> RecordsSet;
public:

    // Iterators
    typedef typename std::unordered_set<DataStorageRecordRef>::iterator iterator;
    typedef typename std::unordered_set<DataStorageRecordRef>::const_iterator const_iterator;

    // Begin and end functions to work with iterators and foreach loop
    inline iterator begin() noexcept { return RecordsSet.begin(); }
    inline const_iterator cbegin() const noexcept { return RecordsSet.cbegin(); }
    inline iterator end() noexcept { return RecordsSet.end(); }
    inline const_iterator cend() const noexcept { return RecordsSet.cend(); }

    void AddNewRecordPtr(const DataStorageRecordRef& newRecordRefPtr)
    {
        RecordsSet.emplace(newRecordRefPtr);
    }

    void AddNewRecordPtr(DataStorageRecord* newRecordPtr, DataStorageStruct *dataStorageStructure)
    {
        RecordsSet.emplace(DataStorageRecordRef(newRecordPtr, dataStorageStructure));
    }
};


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

    // Unordered set with all DataStorageRecord pointers
    std::unordered_set<DataStorageRecord*> RecordsSet;
public:

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
                delete *(std::unordered_multimap<T, DataStorageRecord*>**)ptr;
            }
        );

        // Add function to DataStorageRecord creation
        DataStorageRecordAdders.emplace(keyName, [=](DataStorageRecord* newRecord)
            {
                TtoDataStorageRecordMap->emplace(defaultKeyValue, newRecord);
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
        RecordTemplate.DeleteData(keyName);
        DataStorageStructure.DeleteData(keyName);
        DataStorageRecordAdders.erase(keyName);

        for (auto& it : RecordsSet)
            it->DeleteData(keyName);
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
    DataStorageRecordSet GetRecords(const std::string& keyName, const T& keyValue)
    {
        DataStorageRecordSet res;

        // Pointer to store map inside DataStorageStruct
        std::unordered_multimap<T, DataStorageRecord*>* TtoDataStorageRecordMap = nullptr;
        
        // Checking whether such a key exists
        if (DataStorageStructure.GetData(keyName, TtoDataStorageRecordMap))
        {
            // Iterator to element with T type and keyValue value
            auto FirstAndLastIteratorsWithKey = TtoDataStorageRecordMap->equal_range(keyValue);
            
            // Fill the result record set
            for (auto& it = FirstAndLastIteratorsWithKey.first; it != FirstAndLastIteratorsWithKey.second; ++it)
                res.AddNewRecordPtr(it->second, &DataStorageStructure);
        }

        return res;
    }

    ~DataStorage()
    {
        // Clear DataStorageStructure
        for (auto& it : DataStorageStructure)
            it.second.DeleteData();
        
        // Clear all records
        for (auto& it : RecordsSet)
            delete it;
    }
};

int main()
{
    // Known issues: data saver cannot store arrays

    std::cout << "Phase 1. Simple demo" << std::endl;

    DataStorage ds;
    ds.AddKey("id", -1);
    ds.AddKey<std::string>("name", "none");

    DataStorageRecordRef dsrr = ds.CreateNewRecord();

    if (ds.GetRecord<int>("id", -1, dsrr))
    {
        std::string res;
        if(dsrr.GetData("name", res))
            std::cout << "1: " << res << std::endl;
    }

    dsrr.SetData("id", 0);
    dsrr.SetData<std::string>("name", "mrognor");

    if (ds.GetRecord<int>("id", -1, dsrr))
    {
        std::string res;
        if(dsrr.GetData("name", res))
            std::cout << "2: " << res << std::endl;
    }

    if (ds.GetRecord<int>("id", 0, dsrr))
    {
        std::string res;
        if(dsrr.GetData("name", res))
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
        if(dsrr.GetData("name", res))
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
    
    DataStorageRecordSet dsrs = ds.GetRecords("gender", true);

    for (auto it : dsrs)
    {
        int res;
        if(it.GetData("id", res))
            std::cout << "True gender: " << res << std::endl;
    }

    dsrs = ds.GetRecords("gender", false);

    for (auto it : dsrs)
    {
        int res;
        if(it.GetData("id", res))
            std::cout << "False gender: " << res << std::endl;
    }
}