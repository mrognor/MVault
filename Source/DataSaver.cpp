#include "DataSaver.h"

namespace mvlt
{
    DataSaver::DataSaver() noexcept : DataType(typeid(void)) {}

    DataSaver::DataSaver(const DataSaver& other) noexcept : DataType(typeid(void))
    {
        // Call operator= method
        *this = other;
    }

    DataSaver& DataSaver::operator=(const DataSaver& other) noexcept
    {
        // We check that there is no self-bonding
        if (&other != this)
        {
            // Clear Ptr if it was data before
            if (Ptr != nullptr)
                DeleteFunc(Ptr);

            // Check that other DataSaver is not empty
            if (other.Ptr != nullptr)
            {
                // Create new DataType object copying data type from other
                DataType = other.DataType;

                // Set new CopyFunc from other
                CopyFunc = other.CopyFunc;

                // Call new copy func to copy data from other void pointer to local void pointer. Data to Ptr will be allocated inside function
                CopyFunc(Ptr, other.Ptr);

                // Copy to string function
                ToStringFunc = other.ToStringFunc;

                // Set delete function from other
                DeleteFunc = other.DeleteFunc;

                // Set custom delete function from other
                CustomDeleteFunc = other.CustomDeleteFunc;

                SetDataFromStringFunc = other.SetDataFromStringFunc;
            }
            else
            {
                // If other is empty clear the variables
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

    void DataSaver::Swap(DataSaver& other) noexcept
    {
        DataSaver tmp = other;
        other = *this;
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