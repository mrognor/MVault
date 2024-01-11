#include "DataStorage.h"

void PrintDataStorageRecord(const DataStorageRecordRef& dsrr)
{
    if (dsrr.IsValid())
    {
        int id;
        std::string name;
        bool gender;

        dsrr.GetData("id", id);
        dsrr.GetData("name", name);
        dsrr.GetData("gender", gender);

        std::cout << "Data storage record " << dsrr.GetRecordUniqueId() << " :" << std::endl;
        std::cout << "\tid = " << id << std::endl;
        std::cout << "\tname = " << name << std::endl;
        /// \todo Remove boolalpha
        std::cout << "\tgender = " << std::boolalpha << gender << std::endl;
    }
    else
        std::cout << "Data storage record ref is invalid" << std::endl;
}

int main()
{
    DataStorage ds;

    ds.SetKey("id", -1);
    ds.SetKey<std::string>("name", "");
    ds.SetKey("gender", true);

    ds.CreateRecord({ {"id", 0}, {"name", std::string("A")}, {"gender", true}});
    ds.CreateRecord({ {"id", 1}, {"name", std::string("B")}, {"gender", false}});
    ds.CreateRecord({ {"id", 2}, {"name", std::string("C")}, {"gender", true}});
    ds.CreateRecord({ {"id", 3}, {"name", std::string("D")}, {"gender", false}});

    DataStorageRecordRef dsrr = ds.GetRecord("id", 2);
    if (dsrr.IsValid())
        PrintDataStorageRecord(dsrr);

    dsrr = ds.GetRecord<std::string>("name", "D");
    if (dsrr.IsValid())
        PrintDataStorageRecord(dsrr);
    
    ds.EraseRecord(dsrr);
    std::cout << dsrr.IsValid() << std::endl;
}