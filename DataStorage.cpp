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

public:
    DataSaver() {}

    template<class T>
    DataSaver(T data)
    {
        SetData(data);
    }

    template <class T>
    void SetData(T data)
    {
        if (Ptr != nullptr)
            free(Ptr);
        
        if (DataType != nullptr)
            delete DataType;

        Ptr = malloc(sizeof(T));

        if (Ptr != nullptr)
        {
            memcpy(Ptr, (const void*)&data, sizeof(data));
            DataType = new DataTypeSaver(typeid(data));
        }
    }

    template <class T>
    bool GetData(T& data)
    {
        if (DataType->GetDataType() != typeid(data))
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
        if (Ptr != nullptr)
            free(Ptr);

        if (DataType != nullptr)
        {
            delete DataType;
            DataType = nullptr;
        }
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
        Data.emplace(key, data);
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
        return Data.find(key)->second.GetData(data);
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
    // int a = 24;
    // std::string b = "avf";

    // DataSaver d;

    // d.SetData(a);

    // if (d.GetData(a))
    //     std::cout << a << std::endl;

    // d.SetData(b);

    // if (d.GetData(b))
    //     std::cout << b << std::endl;
    
    // DataSaver c;
    // if (d.GetData(c))
    //     std::cout << b << std::endl;

    DataContainer dc;

    dc.AddData("a", 1);
    dc.AddData("b", 2.2);
    dc.AddData("c", "string");

    int a = 0;
    dc.GetData("a", a);
    std::cout << "Finding a: " << a << std::endl;

    dc.SetData("a", 2);
    dc.GetData("a", a);
    std::cout << "New a: " << a << std::endl;

    std::cout << "Is a: " << dc.IsData("a") << std::endl;
    dc.DeleteData("a");
    std::cout << "Is a: " << dc.IsData("a") << std::endl;
}