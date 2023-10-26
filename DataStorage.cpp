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
            delete DataType;
    }
};

int main()
{
    int a = 24;
    std::string b = "avf";

    DataSaver d;

    d.SetData(a);

    if (d.GetData(a))
        std::cout << a << std::endl;

    d.SetData(b);

    if (d.GetData(b))
        std::cout << b << std::endl;
    
    DataSaver c;
    if (d.GetData(c))
        std::cout << b << std::endl;
}