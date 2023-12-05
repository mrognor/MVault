#include "DataStorage.h"

int main()
{
    // Known limitations: data saver cannot store arrays

    std::cout << "Phase 1. Simple demo" << std::endl;

    DataStorage ds;
    ds.SetKey("id", -1);
    ds.SetKey<std::string>("name", "none");

    DataStorageRecordRef dsrr = ds.CreateNewRecord();

    if (ds.GetRecord<int>("id", -1, dsrr))
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "1: " << res << std::endl;
    }

    dsrr.SetData("id", 0);
    dsrr.SetData<std::string>("name", "mrognor");

    if (ds.GetRecord<int>("id", -1, dsrr))
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "2: " << res << std::endl;
    }

    if (ds.GetRecord<int>("id", 0, dsrr))
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "3: " << res << std::endl;
    }

    if (ds.GetRecord<std::string>("name", "mrognor", dsrr))
    {
        int res;
        if (dsrr.GetData("id", res))
            std::cout << "4: " << res << std::endl;
    }

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 1);
    dsrr.SetData<std::string>("name", "moop");

    if (ds.GetRecord<int>("id", 1, dsrr))
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "5: " << res << std::endl;
    }

    if (ds.GetRecord<std::string>("name", "moop", dsrr))
    {
        int res;
        if (dsrr.GetData("id", res))
            std::cout << "6: " << res << std::endl;
    }

    dsrr.SetData("id", 2);
    if (ds.GetRecord<std::string>("name", "moop", dsrr))
    {
        int res;
        if (dsrr.GetData("id", res))
            std::cout << "7: " << res << std::endl;
    }
    dsrr.SetData("id", 1);

    std::cout << "Phase 2. Runtime key addiction" << std::endl;

    ds.SetKey("gender", true);

    if (ds.GetRecord<bool>("gender", true, dsrr))
    {
        bool res;
        if (dsrr.GetData("gender", res))
        {
            if (res) std::cout << "1: true" << std::endl;
            else std::cout << "1: false" << std::endl;
        }
    }

    std::cout << "Phase 3. Runtime key remove" << std::endl;

    ds.RemoveKey("name");

    if (ds.GetRecord<std::string>("name", "moop", dsrr))
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "1: " << res << std::endl;
    }
    else
        std::cout << "1: No data with name key" << std::endl;

    std::cout << "Phase 4. Getting set of records using key" << std::endl;

    ds.GetRecord("id", 0, dsrr);
    dsrr.SetData("gender", false);

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 2);

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 3);
    dsrr.SetData("gender", false);

    DataStorageRecordSet dsrs;
    ds.GetRecords("gender", true, dsrs);

    for (auto it : dsrs)
    {
        int res;
        if (it.GetData("id", res))
            std::cout << "True gender: " << res << std::endl;
    }

    ds.GetRecords("gender", false, dsrs);

    for (auto it : dsrs)
    {
        int res;
        if (it.GetData("id", res))
            std::cout << "False gender: " << res << std::endl;
    }

    std::cout << "Phase 5. Clear data storage" << std::endl;

    ds.DropData();

    ds.GetRecords("gender", false, dsrs);
    std::cout << "False gender amount: " << dsrs.Size() << std::endl;

    std::cout << "Phase 5. Sets: union, except, intersection, alterintersection" << std::endl;

    ds.DropDataStorage();

    DataStorageRecordSet opset, set0, set1, set2, set3;

    ds.SetKey("id", -1);
    ds.SetKey("type", -1);

    for (int i = 0; i < 20; ++i)
    {
        dsrr = ds.CreateNewRecord();
        dsrr.SetData("id", i);
        dsrr.SetData("type", i % 4);
    }

    ds.GetRecords("type", 0, set0);
    std::cout << "Set0: ";
    for (auto it : set0)
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    ds.GetRecords("type", 1, set1);
    std::cout << "Set1: ";
    for (auto it : set1)
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    ds.GetRecords("type", 2, set2);
    std::cout << "Set2: ";
    for (auto it : set2)
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    ds.GetRecords("type", 3, set3);
    std::cout << "Set3: ";
    for (auto it : set3)
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "Union02: ";
    for (auto it : Union(set0, set2))
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "Union03: ";
    for (auto it : Union(set0, set3))
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "Intersection Union01 Union12: ";
    for (auto it : Intersection(Union(set0, set1), Union(set1, set2)))
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "AlterIntersection Union01 Union12: ";
    for (auto it : AlterIntersection(Union(set0, set1), Union(set1, set2)))
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "Except 0 Union01: ";
    for (auto it : Except(Union(set0, set1), set0))
    {
        int id;
        if (it.GetData("id", id))
            std::cout << id << " ";
    }
    std::cout << std::endl;

    std::cout << "Phase 6. Data erasing" << std::endl;

    ds.DropDataStorage();

    ds.SetKey("id", -1);
    ds.SetKey("type", -1);

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 0);
    dsrr.SetData("type", 0);

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 1);
    dsrr.SetData("type", 0);

    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 1);
    dsrr.SetData("type", 0);

    std::cout << "Size before removing: " << ds.Size() << std::endl;

    ds.EraseRecord(dsrr);

    std::cout << "Size after removing one last element: " << ds.Size() << std::endl;

    ds.GetRecords("type", 0, dsrs);
    
    ds.EraseRecords(dsrs);
    std::cout << "Size after removing set: " << ds.Size() << std::endl;

    std::cout << "Phase 7. RecordRef invalidation" << std::endl;

    ds.DropDataStorage();

    ds.SetKey("id", -1);
    
    dsrr = ds.CreateNewRecord();
    dsrr.SetData("id", 0);
    ds.EraseRecord(dsrr);

    std::cout << dsrr.IsValid() << std::endl;
}