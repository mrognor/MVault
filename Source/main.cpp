#include "DataStorage.h"

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
    ds.PrintAsTable();

    DataStorageRecordRef dsrr = ds.GetRecord("id", 2);
    if (dsrr.IsValid()) dsrr.PrintRecord();

    dsrr = ds.GetRecord<std::string>("name", "D");
    if (dsrr.IsValid()) dsrr.PrintRecord();
        
    std::cout << dsrr.IsValid() << std::endl; 
    ds.EraseRecord(dsrr);
    std::cout << dsrr.IsValid() << std::endl;
}