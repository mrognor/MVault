#include "DataStorageRecord.h"

DataStorageRecord::DataStorageRecord() {}

DataStorageRecord::DataStorageRecord(const DataStorageRecord& recordTemplate)
{
    *this = recordTemplate;
    IsDataStorageRecordValid.SetData(true);
}

DataStorageRecord::~DataStorageRecord()
{
    IsDataStorageRecordValid.SetData(false);
}

DataStorageRecordRef::DataStorageRecordRef() {}

DataStorageRecordRef::DataStorageRecordRef(DataStorageRecord* data, DataStorageStructureHashMap* dataStorageStructureHashMap, DataStorageStructureMap* dataStorageStructureMap) : 
    DataRecord(data), DataStorageHashMapStructure(dataStorageStructureHashMap), DataStorageMapStructure(dataStorageStructureMap)
{
    IsDataStorageRecordValid = data->IsDataStorageRecordValid;
}

bool DataStorageRecordRef::operator==(const DataStorageRecordRef& other) const
{
    return DataRecord == other.DataRecord;
}

std::string DataStorageRecordRef::GetRecordUniqueId() const
{
    std::stringstream ss;
    ss << DataRecord;
    return ss.str();
}

void DataStorageRecordRef::SetData(std::vector<std::pair<std::string, DataSaver>> params)
{
        // Copy data from function parametrs
    for (auto& it : params)
        if (DataRecord->IsData(it.first))
            DataRecord->SetDataFromDataSaver(it.first, it.second);
}

bool DataStorageRecordRef::IsValid() const
{
    return IsDataStorageRecordValid.GetData();
}

void DataStorageRecordRef::Unlink()
{
    DataRecord = nullptr;
    DataStorageHashMapStructure = nullptr;
    DataStorageMapStructure = nullptr;

    IsDataStorageRecordValid.Unlink();
}