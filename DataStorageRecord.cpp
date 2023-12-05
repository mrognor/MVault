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

DataStorageRecordRef::DataStorageRecordRef(DataStorageRecord* data, DataStorageStruct* dataStorageStructure) : DataRecord(data), DataStorageStructure(dataStorageStructure) 
{
    IsDataStorageRecordValid = data->IsDataStorageRecordValid;
}

bool DataStorageRecordRef::operator==(const DataStorageRecordRef& other) const
{
    return DataRecord == other.DataRecord;
}

bool DataStorageRecordRef::IsValid()
{
    return IsDataStorageRecordValid.GetData();
}

void DataStorageRecordRef::Unlink()
{
    DataStorageRecord* DataRecord = nullptr;
    DataStorageStruct* DataStorageStructure = nullptr;
    IsDataStorageRecordValid.Unlink();
}