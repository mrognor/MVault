#include "DataSaver.h"

DataTypeSaver::DataTypeSaver(const std::type_info& type) : TypeInfo(type) {}

const std::type_info& DataTypeSaver::GetDataType() { return TypeInfo; }

DataSaver::DataSaver() {}

DataSaver::DataSaver(const DataSaver& dataSaver)
{
    // Call operator= method
    *this = dataSaver;
}

DataSaver& DataSaver::operator=(const DataSaver& dataSaver)
{
    // We check that there is no self-bonding
    if (&dataSaver != this)
    {
        // Clear Ptr if it was data before
        if (Ptr != nullptr)
            DeleteFunc(Ptr);

        // Clear DataType if it was data before
        if (DataType != nullptr)
            delete DataType;

        // Check that dataSaver is not empty
        if (dataSaver.Ptr != nullptr)
        {
            // Create new DataType object copying data type from dataSaver
            DataType = new DataTypeSaver(dataSaver.DataType->GetDataType());
            
            // Set new CopyFunc from dataSaver
            CopyFunc = dataSaver.CopyFunc;
            
            // Call new copy func to copy data from dataSaver void pointer to local void pointer. Data to Ptr will be allocated inside function
            CopyFunc(Ptr, dataSaver.Ptr);

            // Copy to string function
            ToStringFunc = dataSaver.ToStringFunc;

            // Copy from string function
            FromStringFunc = dataSaver.FromStringFunc;

            // Set delete function from dataSaver
            DeleteFunc = dataSaver.DeleteFunc;
            
            // Set custom delete function from dataSaver
            CustomDeleteFunc = dataSaver.CustomDeleteFunc;
        }
        else
        {
            // If dataSaver is empty clear the variables
            Ptr = nullptr;
            DataType = nullptr;
        }
    }

    return *this;
}

void DataSaver::ResetData()
{
    if (CustomDeleteFunc != nullptr)
    {
        CustomDeleteFunc(Ptr);
        CustomDeleteFunc = nullptr;
    }

    if (Ptr != nullptr)
    {
        DeleteFunc(Ptr);
        Ptr = nullptr;
    }

    if (DataType != nullptr)
    {
        delete DataType;
        DataType = nullptr;
    }

    CopyFunc = nullptr;
    ToStringFunc = nullptr;
    FromStringFunc = nullptr;
}

void DataSaver::Swap(DataSaver& dataSaver)
{
    DataSaver tmp = dataSaver;
    dataSaver = *this;
    *this = tmp;
}

std::string DataSaver::Str() const
{
    return ToStringFunc(Ptr);
}

void DataSaver::SetDataFromString(const std::string stringToCopyDataFrom)
{
    FromStringFunc(stringToCopyDataFrom, Ptr);
}

DataSaver::~DataSaver()
{
    if (Ptr != nullptr)
        DeleteFunc(Ptr);

    if (DataType != nullptr)
        delete DataType;
}