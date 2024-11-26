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

    mvlt::VaultRecordRef vltrr;
    vlt.GetRecord("id", 2, vltrr);
    if (vltrr.IsValid()) vltrr.PrintRecord();

    vlt.GetRecord<std::string>("name", "D", vltrr);
    if (vltrr.IsValid()) vltrr.PrintRecord();
        
    std::cout << vltrr.IsValid() << std::endl; 
    vlt.EraseRecord(vltrr);
    std::cout << vltrr.IsValid() << std::endl;
    std::cout << vltrr.GetRecordUniqueId() << std::endl;

    std::type_index ti = typeid(void);
    vlt.GetKeyType("slaves", ti);
    std::cout << ti.name() << std::endl;

    vlt.PrintAsTable();
    vlt.EraseRecord("id", 1);
    vlt.PrintAsTable();
    vlt.PrintAsTable();

    vltrr = vlt.CreateRecord();

    vltrr.SetData({ {"id", 112}, {"name", std::string("sg")}, {"slaves", std::vector<int>{300, 30, 3}} });
    vltrr.PrintRecord();

    vlt.EraseRecords("slaves", std::vector<int>());

    vlt.DropVault();

    vlt.AddKey("id", -1);
    vlt.CreateRecord();
    vlt.PrintAsTable(true);
    vlt.UpdateKey("id", 0);
    vlt.CreateRecord();
    vlt.PrintAsTable(true);

    vlt.DropVault();
    vlt.AddKey("A", -1);
    vlt.AddKey<std::string>("B", "");
    vlt.AddKey("C", false);

    vlt.CreateRecord();
    vlt.CreateRecord();
    vlt.CreateRecord();
    vlt.CreateRecord();
    vlt.CreateRecord();
    vlt.CreateRecord();
    
    vlt.PrintAsTable();
    
    vlt.AddUniqueKey<std::string>("D", {[](const std::size_t& count) -> std::string 
    { 
        switch (count % 3)
        {
        case 0:
            return "Zero";
            break;
        case 1:
            return "One";
            break;
        case 2:
            return "Two";
            break;
        }

        return "None";
    }});
    vlt.PrintAsTable();
    vlt.CreateRecord();
}
