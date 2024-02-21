#include "DataSaver.h"

namespace mvlt
{
    DataSaver::DataSaver() : DataType(typeid(void)) {}

    DataSaver::DataSaver(const DataSaver& dataSaver) : DataType(typeid(void))
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
            }
            else
            {
                // If dataSaver is empty clear the variables
                Ptr = nullptr;
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

        DataType = typeid(void);
        CopyFunc = nullptr;
        ToStringFunc = nullptr;
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

    std::type_index DataSaver::GetDataType() const
    {
        return DataType;
    }

    DataSaver::~DataSaver()
    {
        if (Ptr != nullptr)
            DeleteFunc(Ptr);
    }
}