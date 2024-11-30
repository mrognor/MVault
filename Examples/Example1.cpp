#include "../Source/MVault.h"

template <>
struct std::hash<std::vector<int>>
{
    std::size_t operator()(const std::vector<int>& vec) const noexcept
    {
        std::size_t seed = vec.size();
  		for(auto& i : vec)
    		seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  		return seed;
    }
};

/// \todo Добавить описание этой функции в доки
template <>
std::string mvlt::ToString(const std::vector<int>& data)  noexcept
{
    if (data.size() == 0) return "{}";
    std::string res = "{";
    for (const int& dataIt : data)
        res += std::to_string(dataIt) + ", ";


    res.pop_back();
    res.pop_back();

    res += "}";
    return res; 
}

int main()
{
    mvlt::Vault vlt;

    vlt.AddKey("id", -1);
    vlt.AddKey<std::string>("name", "");
    vlt.AddKey("gender", true);
    vlt.AddKey("slaves", std::vector<int>());

    vlt.CreateRecord({ {"id", 0}, {"name", std::string("A")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 1}, {"name", std::string("B")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 2}, {"name", std::string("C")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 3}, {"name", std::string("D")}, {"gender", false}, {"slaves", std::vector<int>{1, 2, 3}} });
    vlt.PrintAsTable();

    mvlt::VaultRecordRef vrf;
    vlt.GetRecord("id", 2, vrf);
    if (vrf.IsValid()) vrf.PrintRecord();

    vlt.GetRecord<std::string>("name", "D", vrf);
    if (vrf.IsValid()) vrf.PrintRecord();
        
    std::cout << vrf.IsValid() << std::endl; 
    vlt.EraseRecord(vrf);
    std::cout << vrf.IsValid() << std::endl;
    std::cout << vrf.GetRecordUniqueId() << std::endl;

    std::type_index ti = typeid(void);
    vlt.GetKeyType("slaves", ti);
    std::cout << ti.name() << std::endl;

    vlt.PrintAsTable();
    vlt.EraseRecord("id", 1);
    vlt.PrintAsTable();
    vlt.PrintAsTable();

    vlt.CreateRecord(vrf, {});

    vrf.SetData({ {"id", 112}, {"name", std::string("sg")}, {"slaves", std::vector<int>{300, 30, 3}} });
    vrf.PrintRecord();

    vlt.EraseRecords("slaves", std::vector<int>());

    vlt.DropVault();

    vlt.AddKey("id", -1);
    vlt.CreateRecord({});
    vlt.PrintAsTable(true);
    vlt.UpdateKey("id", 0);
    vlt.CreateRecord({});
    vlt.PrintAsTable(true);

    vlt.DropVault();
    vlt.AddKey("A", -1);
    vlt.AddKey<std::string>("B", "");
    vlt.AddKey("C", false);

    for (int i = 0; i < 10; ++i)
    {
        vlt.CreateRecord({{"A", i * i}});
    }
    
    vlt.PrintAsTable();
    
    vlt.AddUniqueKey<std::string>("D", {[&](const std::size_t& counter, const mvlt::VaultRecordRef& vrf) -> std::string 
    {
        int i;
        vrf.GetData("A", i);
        return "Number: " + std::to_string(i * 2) + " index: " + std::to_string(counter);
    }});

    vlt.PrintAsTable();
    vlt.CreateRecord({});
}
