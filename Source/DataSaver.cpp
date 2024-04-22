#include "DataSaver.h"

namespace mvlt
{
    DataSaver::DataSaver() noexcept : DataType(typeid(void)) {}

    DataSaver::DataSaver(const DataSaver& dataSaver) noexcept : DataType(typeid(void))
    {
        // Call operator= method
        *this = dataSaver;
    }

    DataSaver& DataSaver::operator=(const DataSaver& dataSaver) noexcept
    {
        // We check that there is no self-bonding
        if (&dataSaver != this)
        {
            // Clear Ptr if it was data before
            if (Ptr != nullptr)
                DeleteFunc(Ptr);

            // Check that dataSaver is not empty
            if (dataSaver.Ptr != nullptr)
            {
                // Create new DataType object copying data type from dataSaver
                DataType = dataSaver.DataType;
                
                // Set new CopyFunc from dataSaver
                CopyFunc = dataSaver.CopyFunc;
                
                // Call new copy func to copy data from dataSaver void pointer to local void pointer. Data to Ptr will be allocated inside function
                CopyFunc(Ptr, dataSaver.Ptr);

                // Copy to string function
                ToStringFunc = dataSaver.ToStringFunc;

                // Set delete function from dataSaver
                DeleteFunc = dataSaver.DeleteFunc;
                
                // Set custom delete function from dataSaver
                CustomDeleteFunc = dataSaver.CustomDeleteFunc;

                SetDataFromStringFunc = dataSaver.SetDataFromStringFunc;
            }
            else
            {
                // If dataSaver is empty clear the variables
                Ptr = nullptr;
            }
        }

        return *this;
    }

    bool DataSaver::SetDataFromString(const std::string& data) noexcept
    {
        return SetDataFromStringFunc(Ptr, data);
    }

    void DataSaver::ResetData() noexcept
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

        DataType = typeid(void);
        CopyFunc = nullptr;
        ToStringFunc = nullptr;
        SetDataFromStringFunc = nullptr;
    }

    void DataSaver::Swap(DataSaver& dataSaver) noexcept
    {
        DataSaver tmp = dataSaver;
        dataSaver = *this;
        *this = tmp;
    }

    std::string DataSaver::Str() const noexcept
    {
        return ToStringFunc(Ptr);
    }

    std::type_index DataSaver::GetDataType() const noexcept
    {
        return DataType;
    }

    DataSaver::~DataSaver() noexcept
    {
        if (Ptr != nullptr)
            DeleteFunc(Ptr);
    }
}