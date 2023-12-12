#include "DataStorage.h"

void PrintDataStorage(DataStorage& ds)
{
    // id len = 2
    std::size_t idLen = 2;
    // name len = 4
    std::size_t nameLen = 4;
    // No need to check. gender longer than woman or man
    std::size_t genderLen = 6;

    DataStorageRecordSet allRecords = ds.GetAllRecords();

    for (auto& it : allRecords)
    {
        int id;
        std::string name;

        it.GetData("id", id);
        it.GetData("name", name);

        if (std::to_string(id).size() > idLen)
            idLen = std::to_string(id).size();
        
        if (name.size() > nameLen)
            nameLen = name.size();
    }

    // Print first line
    std::cout << "\t=";
    for (std::size_t i = 0; i < idLen; ++i)
        std::cout << "=";
    
    std::cout << "=";
    for (std::size_t i = 0; i < nameLen; ++i)
        std::cout << "=";

    std::cout << "=";
    for (std::size_t i = 0; i < genderLen; ++i)
        std::cout << "=";

    std::cout << "=" << std::endl;

    // Print keys
    std::cout << "\t|id";
    for (std::size_t i = 0; i < idLen - 2; ++i)
        std::cout << " ";
    
    std::cout << "|name";
    for (std::size_t i = 0; i < nameLen - 4; ++i)
        std::cout << " ";

    std::cout << "|gender|";
    std::cout << std::endl;

    // Print third line
    std::cout << "\t+";
    for (std::size_t i = 0; i < idLen; ++i)
        std::cout << "-";
    
    std::cout << "+";
    for (std::size_t i = 0; i < nameLen; ++i)
        std::cout << "-";

    std::cout << "+";
    for (std::size_t i = 0; i < genderLen; ++i)
        std::cout << "-";

    std::cout << "+" << std::endl;
    
    for (auto& it : allRecords)
    {
        int id;
        std::string name;
        bool gender;
        it.GetData("id", id);
        it.GetData("name", name);
        it.GetData("gender", gender);

        std::cout << "\t|" << id;
        for (std::size_t i = 0; i < idLen - std::to_string(id).size(); ++i)
            std::cout << " ";

        std::cout << "|" << name;
        for (std::size_t i = 0; i < nameLen - name.size(); ++i)
            std::cout << " ";

        std::cout << "|";
        if (gender)
        {
            std::cout << "man";
            for (std::size_t i = 0; i < genderLen - 3; ++i)
                std::cout << " ";
        }
        else
        {
            std::cout << "woman";
            for (std::size_t i = 0; i < genderLen - 5; ++i)
                std::cout << " ";
        }
        std::cout << "|" << std::endl;
    }

    // Print last line
    std::cout << "\t=";
    for (std::size_t i = 0; i < idLen; ++i)
        std::cout << "=";
    
    std::cout << "=";
    for (std::size_t i = 0; i < nameLen; ++i)
        std::cout << "=";

    std::cout << "=";
    for (std::size_t i = 0; i < genderLen; ++i)
        std::cout << "=";

    std::cout << "=" << std::endl;
    std::cout << "\tNumber of records: " << ds.Size() << std::endl;
}

void PrintDataStorageRecord(const DataStorageRecordRef& recRef)
{
    int id;
    std::string name;
    bool gender;
    recRef.GetData("id", id);
    recRef.GetData("name", name);
    recRef.GetData("gender", gender);
    std::cout << "\tRecord unique id: " << recRef.GetRecordUniqueId() << std::endl;
    std::cout << "\tid: " << id << std::endl;
    std::cout << "\tname: " << name << std::endl;
    if (gender)
        std::cout << "\tgender: man" << std::endl;
    else
        std::cout << "\tgender: man" << std::endl;
}

void Example1(DataStorage& ds)
{
    std::cout << "Example 1. Simple addition and deletion of records" << std::endl;

    // ================================================================ //
    // Adding three keys: int id, string name, string surname
    ds.SetKey("id", -1);                     // Column name: id. Default value: -1. Type: int
    ds.SetKey<std::string>("name", "none");  // Column name: name. Default value: none. Type: std::string
    ds.SetKey<bool>("gender", true);         // Column name: gender. Default value: true. Type: bool. True = man, False = woman

    /*
        The table will look like this:
        | id | name  |  gender |
        +----+------+----------+
    */

    // Print data storage
    std::cout << "Empty data storage" << std::endl;
    PrintDataStorage(ds);

    // ================================================================ //

    // Creating a record, all keys will have default values
    ds.CreateRecord();

    /*
        The table will look like this:
        | id | name  |  gender |
        +----+-------+---------+
        | -1 | none  |   man   |
    */

    // Print data storage
    std::cout << "Data storage after the first addition" << std::endl;
    PrintDataStorage(ds);

    // ================================================================ //

    // Adding a record with passing the values of all keys through the function
    ds.CreateRecord( { {"id", 0}, {"name", std::string("mrognor")}, {"gender", true} } );
    
    /*
        The table will look like this:
        | id | name  |  gender |
        +----+-------+---------+
        | -1 | none  |   man   |
        |  0 |mrognor|   man   |
    */

    // Print data storage
    std::cout << "Data storage after the second addition" << std::endl;
    PrintDataStorage(ds);

    // ================================================================ //


    // Adding a record with key values passing through the function. The gender value remains the default
    ds.CreateRecord( { {"name", std::string("ltrttl")}, {"id", 1} } );
    
    /*
        The table will look like this:
        | id | name  |  gender |
        +----+-------+---------+
        | -1 | none  |   man   |
        |  0 |mrognor|   man   |
        |  1 |ltrttl |   man   |
    */

    // Print data storage
    std::cout << "Data storage after the third addition" << std::endl;
    PrintDataStorage(ds);

    // ================================================================ //

    // Adding an record. Key values are passed through a vector
    std::vector<std::pair<std::string, DataSaver>> recordParams = { {"id", 2}, {"gender", false} };
    recordParams.emplace_back(std::pair<std::string, DataSaver>("name", std::string("ada")));

    ds.CreateRecord(recordParams);

    /*
        The table will look like this:
        | id | name  |  gender |
        +----+-------+---------+
        | -1 | none  |   man   |
        |  0 |mrognor|   man   |
        |  1 |ltrttl |   man   |
        |  2 |  ada  |  woman  |
    */

    // Print data storage
    std::cout << "Data storage after the last addition" << std::endl;
    PrintDataStorage(ds);

    // ================================================================ //

    // Get record with name mrognor
    std::cout << "The result of a request for a record with the name mrognor" << std::endl;

    DataStorageRecordRef recordRef = ds.GetRecord("name", std::string("mrognor"));
    
    if (recordRef.IsValid())
        PrintDataStorageRecord(recordRef);
    
    // Invalidate ref
    recordRef.Unlink();

    // ================================================================ //

    // Drop table
    ds.DropDataStorage();
}

int main()
{
    DataStorage ds;
    
    Example1(ds);

    std::cout << "Phase 0. Demo" << std::endl;
    
    ds.SetKey("id", -1);
    ds.SetKey<std::string>("name", "none");

    DataStorageRecordRef dsrr = ds.CreateRecord({{"id", 0}, {"name", std::string("zov")}});

    dsrr = ds.GetRecord<int>("id", 0);

    if (dsrr.IsValid())
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "0: " << res << std::endl;
    }

    dsrr.SetData({{"id", 1}, {"name", std::string("moop")}});
    
    dsrr = ds.GetRecord<int>("id", 0);

    if (dsrr.IsValid())
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "0: " << res << std::endl;
    }

    ds.DropData();

    std::cout << "Phase 1. Simple demo" << std::endl;

    dsrr = ds.CreateRecord();

    dsrr = ds.GetRecord<int>("id", -1);

    if (dsrr.IsValid())
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "1: " << res << std::endl;
    }

    dsrr.SetData("id", 0);
    dsrr.SetData<std::string>("name", "mrognor");

    dsrr = ds.GetRecord<int>("id", -1);

    if (dsrr.IsValid())
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "2: " << res << std::endl;
    }

    dsrr = ds.GetRecord<int>("id", 0);

    if (dsrr.IsValid())
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "3: " << res << std::endl;
    }

    dsrr = ds.GetRecord<std::string>("name", "mrognor");

    if (dsrr.IsValid())
    {
        int res;
        if (dsrr.GetData("id", res))
            std::cout << "4: " << res << std::endl;
    }

    dsrr = ds.CreateRecord();
    dsrr.SetData("id", 1);
    dsrr.SetData<std::string>("name", "moop");

    dsrr = ds.GetRecord<int>("id", 1);

    if (dsrr.IsValid())
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "5: " << res << std::endl;
    }

    dsrr = ds.GetRecord<std::string>("name", "moop");

    if (dsrr.IsValid())
    {
        int res;
        if (dsrr.GetData("id", res))
            std::cout << "6: " << res << std::endl;
    }

    dsrr.SetData("id", 2);
    dsrr = ds.GetRecord<std::string>("name", "moop");

    if (dsrr.IsValid())
    {
        int res;
        if (dsrr.GetData("id", res))
            std::cout << "7: " << res << std::endl;
    }
    dsrr.SetData("id", 1);

    std::cout << "Phase 2. Runtime key addiction" << std::endl;

    ds.SetKey("gender", true);

    dsrr = ds.GetRecord<bool>("gender", true);

    if (dsrr.IsValid())
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

    dsrr = ds.GetRecord<std::string>("name", "moop");

    if (dsrr.IsValid())
    {
        std::string res;
        if (dsrr.GetData("name", res))
            std::cout << "1: " << res << std::endl;
    }
    else
        std::cout << "1: No data with name key" << std::endl;

    std::cout << "Phase 4. Getting set of records using key" << std::endl;

    dsrr = ds.GetRecord("id", 0);
    dsrr.SetData("gender", false);

    dsrr = ds.CreateRecord();
    dsrr.SetData("id", 2);

    dsrr = ds.CreateRecord();
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
        dsrr = ds.CreateRecord();
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

    dsrr = ds.CreateRecord();
    dsrr.SetData("id", 0);
    dsrr.SetData("type", 0);

    dsrr = ds.CreateRecord();
    dsrr.SetData("id", 1);
    dsrr.SetData("type", 0);

    dsrr = ds.CreateRecord();
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
    
    dsrr = ds.CreateRecord();
    dsrr.SetData("id", 0);
    ds.EraseRecord(dsrr);

    std::cout << dsrr.IsValid() << std::endl;

    std::cout << "Phase 8. Requests" << std::endl;
    ds.DropDataStorage();

    ds.SetKey("id", 0);

    for (int i = 0; i < 20; ++i)
        ds.CreateRecord({ {"id", i} });
    
    ds.RequestRecords("id", GreaterOrEqual<int>(10), opset);

    for (auto& it : opset)
    {
        int id;
        it.GetData("id", id);
        std::cout << id << " ";
    }
    std::cout << std::endl;

    ds.RequestRecords("id", Greater<int>(15), opset);

    for (auto& it : opset)
    {
        int id;
        it.GetData("id", id);
        std::cout << id << " ";
    }
    std::cout << std::endl;

    ds.RequestRecords("id", LessOrEqual<int>(10), opset);

    for (auto& it : opset)
    {
        int id;
        it.GetData("id", id);
        std::cout << id << " ";
    }
    std::cout << std::endl;

    ds.RequestRecords("id", Less<int>(3), opset);

    for (auto& it : opset)
    {
        int id;
        it.GetData("id", id);
        std::cout << id << " ";
    }
    std::cout << std::endl;

    ds.RequestRecords("id", Between<int>(15, 5), opset);

    for (auto& it : opset)
    {
        int id;
        it.GetData("id", id);
        std::cout << id << " ";
    }
    std::cout << std::endl;

        ds.RequestRecords("id", Between<int>(5, 15), opset);

    for (auto& it : opset)
    {
        int id;
        it.GetData("id", id);
        std::cout << id << " ";
    }
    std::cout << std::endl;
}