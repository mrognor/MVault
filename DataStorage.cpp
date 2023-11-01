#include <iostream>
#include <unordered_map>
#include <cstring>
#include <list>
#include <typeinfo>

class DataTypeSaver
{
private:
    const std::type_info& TypeInfo;
public:
    DataTypeSaver(const std::type_info& type) : TypeInfo(type) {}
    const std::type_info& GetDataType() { return TypeInfo; }
};

class DataSaver
{
private:
    void* Ptr = nullptr;
    DataTypeSaver* DataType = nullptr;
    std::size_t DataTypeSize = 0;

public:
    DataSaver() 
    {
        std::cout << "Const" << std::endl;
    }

    DataSaver(const DataSaver& dataSaver)
    {
        *this = dataSaver;
    }

    DataSaver& operator=(const DataSaver& dataSaver)
    {
        if (Ptr != nullptr)
            free(Ptr);

        if (DataType != nullptr)
            delete DataType;

        std::copy(dataSaver.Ptr, Ptr);
        return *this;
    }

    template<class T>
    DataSaver(const T& data)
    {
        SetData(data);
    }

    template <class T>
    void SetData(const T& data)
    {
        if (Ptr != nullptr)
            free(Ptr);

        if (DataType != nullptr)
            delete DataType;

        T* tmpPtr = new T(data);
        
        Ptr = (void*)tmpPtr;
        DataType = new DataTypeSaver(typeid(data));
        DataTypeSize = sizeof(T);
    }

    template <class T>
    bool GetData(T& data)
    {
        if (DataType != nullptr && DataType->GetDataType() != typeid(data))
        {
            std::cout << "Wrong type! Was: " + std::string(DataType->GetDataType().name()) + " Requested: " + typeid(data).name() << std::endl;
            return false;
        }

        if (Ptr == nullptr)
            return false;

        data = *(T*)Ptr;
        return true;
    }
       
    ~DataSaver()
    {
        std::cout << "Dest " << Ptr << std::endl;
        if (Ptr != nullptr)
            free(Ptr);

        if (DataType != nullptr)
            delete DataType;
    }

    void Test()
    {
        std::cout << *(int*)Ptr << std::endl;
    }
};

class DataContainer
{
private:
    std::unordered_map<std::string, DataSaver> Data;
public:
    template <class T>
    void AddData(std::string key, const T& data)
    {
        Data.emplace(key, DataSaver(data));
    }

    template <class T>
    void SetData(std::string key, const T& data)
    {
        auto f = Data.find(key);

        if (f == Data.end())
            AddData(key, data);
        else
            f->second.SetData(data);
    }

    template <class T>
    bool GetData(std::string key, T& data)
    {
        auto f = Data.find(key);
        if (f == Data.end())
            return false;

        f->second.GetData(data);
        return true;
    }

    bool IsData(std::string key)
    {
        return Data.find(key) != Data.end();
    }

    void DeleteData(std::string key)
    {
        Data.erase(key);
    }
};

int main()
{
    DataContainer dc;
    dc.AddData("id", 12);
}