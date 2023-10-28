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
    DataSaver() {}

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

        Ptr = malloc(dataSaver.DataTypeSize);

        if (Ptr != nullptr)
        {
            memcpy(Ptr, (const void*)dataSaver.Ptr, dataSaver.DataTypeSize);
            DataType = new DataTypeSaver(*dataSaver.DataType);
            DataTypeSize = dataSaver.DataTypeSize;
        }

        return *this;
    }

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
            memcpy(Ptr, (const void*)&data, sizeof(T));
            DataType = new DataTypeSaver(typeid(data));
            DataTypeSize = sizeof(T);
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
    std::list<DataContainer> la;
    la.emplace_back(DataContainer());
    la.emplace_back(DataContainer());

    auto it = la.begin();
    it->AddData("a", 1);
    it->AddData("b", std::string("azaz"));
    it->AddData("c", 12.32);
    ++it;
    it->AddData("a", 4);
    it->AddData("b", std::string("bazaa"));
    it->AddData("c", 2.44);

    int i;
    std::string s;
    double f;

    it->GetData("b", s);
    std::cout << s << std::endl;

    for (auto it : s)
        std::cout << (int)it << " ";

    for (auto it = la.begin(); it != la.end(); ++it)
    {
        it->GetData("a", i);
        std::cout << "int: " << i << std::endl;

        it->GetData("b", s);
        std::cout << "string: " << s << std::endl;
        for (auto it : s)
            std::cout << (int)it << " ";

        it->GetData("c", f);
        std::cout << "float: " << f << std::endl;
    }

    std::cout << "---" << std::endl;

    for (auto it : la)
    {
        it.GetData("a", i);
        std::cout << "int: " << i << std::endl;

        it.GetData("c", f);
        std::cout << "float: " << f << std::endl;

        it.GetData("b", s);
        std::cout << "string: " << s << std::endl;
    }
}