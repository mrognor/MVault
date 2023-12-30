#include "DataStorageRequests.h"

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