#include <iostream>
#include <unordered_map>
#include <cstring>
#include <list>
#include <typeinfo>

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

    // Template method to save data inside DataSaver
    template <class T>
    void SetData(const T& data)
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

    // Destructor
    ~DataSaver()
    {
        if (Ptr != nullptr)
            free(Ptr);

        if (DataType != nullptr)
            delete DataType;
    }
};

class DataContainer
{
private:
    std::unordered_map<std::string, DataSaver> Data;
public:
    template <class T>
    void AddData(const std::string& key, const T& data)
    {
        Data.emplace(key, DataSaver(data));
    }

    template <class T>
    void SetData(const std::string& key, const T& data)
    {
        auto f = Data.find(key);

        if (f == Data.end())
            AddData(key, data);
        else
            f->second.SetData(data);
    }

    template <class T>
    bool GetData(const std::string& key, T& data)
    {
        auto f = Data.find(key);
        if (f == Data.end())
            return false;

        f->second.GetData(data);
        return true;
    }

    bool IsData(const std::string& key)
    {
        return Data.find(key) != Data.end();
    }

    void DeleteData(const std::string& key)
    {
        Data.erase(key);
    }
};


class DataStorage
{
private:
    DataContainer DataTemplate;
    std::unordered_map<std::string, void*> ParamMaps;
    std::list<DataContainer> ListOfDataContainers;
public:
    template <class T>
    void AddParam(const std::string& paramName, T defaultParamValue)
    {
        DataTemplate.AddData(paramName, defaultParamValue);
        ParamMaps.emplace(paramName, new std::unordered_map<T, std::list<DataContainer>::iterator>()); // memory leak
    }

    void AddElement()
    {
        ListOfDataContainers.emplace_front(DataTemplate);
    }

    // Set last element
    template <class T>
    void SetElement(const std::string& paramName, const T& paramValue)
    {
        auto f = ListOfDataContainers.begin();
        f->SetData(paramName, paramValue);
        
        std::unordered_map<T, std::list<DataContainer>::iterator>* tmpPtr = (std::unordered_map<T, std::list<DataContainer>::iterator>*)ParamMaps.find(paramName)->second;

        auto res = tmpPtr->emplace(paramValue, f);
        if (!res.second)
        {
            tmpPtr->erase(res.first);
            tmpPtr->emplace(paramValue, f);
        }
    }

    template <class T>
    bool GetElement(const std::string& paramName, const T& paramValue, DataContainer& foundedElemIt)
    {
        std::unordered_map<T, std::list<DataContainer>::iterator>* tmpPtr = (std::unordered_map<T, std::list<DataContainer>::iterator>*)ParamMaps.find(paramName)->second;

        auto f = tmpPtr->find(paramValue);

        if (f == tmpPtr->end()) return false;

        foundedElemIt = *f->second;
        return true;
    }

    ~DataStorage()
    {
        for (auto& it : ParamMaps)
        {
            free(it.second);
        }
    }
};

int main()
{
    // Known issues: data saver cannot store arrays
    // Limitations: save pointers in data saver only on your own risk because it is very hard to properly deallocate memory

    // DataSaver ds;
    // int* a = new int(12);

    // ds.SetData(a);
    // int* b;
    // ds.GetData(b);

    // std::cout << *b << std::endl;
    // delete a;


    // DataContainer dc;
    // int* a = new int(10);

    // dc.AddData("a", a);
    // int* b;
    // dc.GetData("a", b);
    // std::cout << *b << std::endl;
    // delete a;

    DataStorage ds;

    ds.AddParam("id", -1);
    ds.AddParam<std::string>("name", "");

    ds.AddElement();
    ds.SetElement("id", 1);
    ds.SetElement<std::string>("name", "mrognor");

    ds.AddElement();
    ds.SetElement("id", 2);
    ds.SetElement<std::string>("name", "moop");

    DataContainer dc;
    ds.GetElement("id", 1, dc);
    
    std::string name;
    if (dc.GetData("name", name))
        std::cout << name << std::endl;
    
    ds.GetElement("id", 2, dc);

    if (dc.GetData("name", name))
        std::cout << name << std::endl;

    ds.SetElement<std::string>("name", "ZmoopZ");
    
    ds.GetElement("id", 2, dc);

    if (dc.GetData("name", name))
        std::cout << name << std::endl;
}