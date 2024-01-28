#include "DataStorage.h"

template <>
struct std::hash<std::vector<int>>
{
    std::size_t operator()(const std::vector<int>& vec) const
    {
        std::size_t seed = vec.size();
  		for(auto& i : vec)
    		seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  		return seed;
    }
};

/// \todo Добавить описание этой функции в доки
template <>
std::string ToString(const std::vector<int>& data) 
{
    if (data.size() == 0) return "{}";
    std::string res = "{";
    for (const int& it : data)
    {
        res += std::to_string(it) + ", ";
    }

    res.pop_back();
    res.pop_back();

    res += "}";
    return res; 
}

int main()
{
    DataStorage ds;

    ds.SetKey("id", -1);
    ds.SetKey<std::string>("name", "");
    ds.SetKey("gender", true);
    ds.SetKey("slaves", std::vector<int>());

    ds.CreateRecord({ {"id", 0}, {"name", std::string("A")}, {"gender", true} });
    ds.CreateRecord({ {"id", 1}, {"name", std::string("B")}, {"gender", false} });
    ds.CreateRecord({ {"id", 2}, {"name", std::string("C")}, {"gender", true} });
    ds.CreateRecord({ {"id", 3}, {"name", std::string("D")}, {"gender", false}, {"slaves", std::vector<int>{1, 2, 3}} });
    ds.PrintAsTable();

    DataStorageRecordRef dsrr = ds.GetRecord("id", 2);
    if (dsrr.IsValid()) dsrr.PrintRecord();

    dsrr = ds.GetRecord<std::string>("name", "D");
    if (dsrr.IsValid()) dsrr.PrintRecord();
        
    std::cout << dsrr.IsValid() << std::endl; 
    ds.EraseRecord(dsrr);
    std::cout << dsrr.IsValid() << std::endl;
}