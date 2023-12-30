#include "DataStorageRecordSet.h"

DataStorageRecordSet::iterator DataStorageRecordSet::begin() noexcept { return RecordsSet.begin(); }

DataStorageRecordSet::const_iterator DataStorageRecordSet::begin() const noexcept { return RecordsSet.begin(); }

DataStorageRecordSet::const_iterator DataStorageRecordSet::cbegin() const noexcept { return RecordsSet.cbegin(); }

DataStorageRecordSet::iterator DataStorageRecordSet::end() noexcept { return RecordsSet.end(); }

DataStorageRecordSet::const_iterator DataStorageRecordSet::end() const noexcept { return RecordsSet.end(); }

DataStorageRecordSet::const_iterator DataStorageRecordSet::cend() const noexcept { return RecordsSet.cend(); }

void DataStorageRecordSet::AddNewRecord(const DataStorageRecordRef& newRecordRefPtr)
{
    RecordsSet.emplace(newRecordRefPtr);
}

void DataStorageRecordSet::AddNewRecord(DataStorageRecord* newRecordPtr, DataStorageStructureHashMap* dataStorageStructureHashMap, DataStorageStructureMap* dataStorageStructureMap)
{
    RecordsSet.emplace(DataStorageRecordRef(newRecordPtr, dataStorageStructureHashMap, dataStorageStructureMap));
}

std::size_t DataStorageRecordSet::Size() const
{
    return RecordsSet.size();
}

void DataStorageRecordSet::Clear()
{
    RecordsSet.clear();
}

bool DataStorageRecordSet::Contain(const DataStorageRecordRef& dataToCheck) const
{
    return RecordsSet.find(dataToCheck) != RecordsSet.end();
}

void DataStorageRecordSet::Join(const DataStorageRecordSet& other)
{
    for (auto& it : other)
        RecordsSet.emplace(it);
}


