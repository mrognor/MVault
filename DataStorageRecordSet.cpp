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

void DataStorageRecordSet::AddNewRecord(DataStorageRecord* newRecordPtr, DataStorageStruct* dataStorageStructure)
{
    RecordsSet.emplace(DataStorageRecordRef(newRecordPtr, dataStorageStructure));
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


DataStorageRecordSet Union(const DataStorageRecordSet& A, const DataStorageRecordSet& B)
{
    DataStorageRecordSet res = A;

    for (auto& it : B)
        res.AddNewRecord(it);
    return res;
};

DataStorageRecordSet Except(const DataStorageRecordSet& A, const DataStorageRecordSet& B)
{
    DataStorageRecordSet res;

    for (auto& it : A)
        if (!B.Contain(it))
            res.AddNewRecord(it);
    return res;
};

DataStorageRecordSet Intersection(const DataStorageRecordSet& A, const DataStorageRecordSet& B)
{
    DataStorageRecordSet res;

    if (A.Size() < B.Size())
    {
        for (auto& it : A)
            if (B.Contain(it))
                res.AddNewRecord(it);
    }
    else
    {
        for (auto& it : B)
            if (A.Contain(it))
                res.AddNewRecord(it);
    }

    return res;
};

DataStorageRecordSet AlterIntersection(const DataStorageRecordSet& A, const DataStorageRecordSet& B)
{
    DataStorageRecordSet res, tmp;

    if (A.Size() < B.Size())
    {
        for (auto& it : A)
        {
            if (B.Contain(it))
                tmp.AddNewRecord(it);
            else
                res.AddNewRecord(it);
        }

        for (auto& it : B)
            if (!tmp.Contain(it))
                res.AddNewRecord(it);
    }
    else
    {
        for (auto& it : B)
        {
            if (A.Contain(it))
                tmp.AddNewRecord(it);
            else
                res.AddNewRecord(it);
        }

        for (auto& it : A)
            if (!tmp.Contain(it))
                res.AddNewRecord(it);
    }

    return res;
};