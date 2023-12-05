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

void DataStorageRecordRef::SetDataStorageRecordPtr(DataStorageRecord* data) { DataRecord = data; }
void DataStorageRecordRef::SetDataStorageStructPtr(DataStorageStruct* dataStorageStructure) { DataStorageStructure = dataStorageStructure; }

bool DataStorageRecordRef::operator==(const DataStorageRecordRef& other) const
{
    return DataRecord == other.DataRecord;
}

DataStorageRecord* DataStorageRecordRef::GetRawData() const
{
    return DataRecord;
}

bool DataStorageRecordRef::IsValid()
{
    return IsDataStorageRecordValid.GetData();
}