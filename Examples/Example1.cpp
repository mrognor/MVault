#include "../Source/Vault.h"

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
std::string mvlt::ToString(const std::vector<int>& data) 
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
    mvlt::Vault vlt;

    vlt.SetKey("id", -1);
    vlt.SetKey<std::string>("name", "");
    vlt.SetKey("gender", true);
    vlt.SetKey("slaves", std::vector<int>());

    vlt.CreateRecord({ {"id", 0}, {"name", std::string("A")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 1}, {"name", std::string("B")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 2}, {"name", std::string("C")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 3}, {"name", std::string("D")}, {"gender", false}, {"slaves", std::vector<int>{1, 2, 3}} });
    vlt.PrintAsTable();

    mvlt::VaultRecordRef vltrr = vlt.GetRecord("id", 2);
    if (vltrr.IsValid()) vltrr.PrintRecord();

    vltrr = vlt.GetRecord<std::string>("name", "D");
    if (vltrr.IsValid()) vltrr.PrintRecord();
        
    std::cout << vltrr.IsValid() << std::endl; 
    vlt.EraseRecord(vltrr);
    std::cout << vltrr.IsValid() << std::endl;
    std::cout << vltrr.GetRecordUniqueId() << std::endl;

    std::type_index ti = typeid(void);
    vlt.GetKeyType("slaves", ti);
    std::cout << ti.name() << std::endl;
}