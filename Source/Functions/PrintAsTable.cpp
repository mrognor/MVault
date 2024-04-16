#include "PrintAsTable.h"

namespace mvlt
{
    std::string GetDataAsString(VaultRecord* record, const std::string& key) noexcept
    {
        std::string res;
        record->GetDataAsString(key, res);
        return res;
    }

    /**
        \brief Function for PrintContainerAsTable

        This function is used as an interface. It is called inside PrintContainerAsTable and allows it to work with VaulRecord and VaultRecordRef and their pointers.

        \param [in] record the record
        \param [in] key the key whose value you want to get from the record

        \return the value of the key in the string
    */
    std::string GetDataAsString(const VaultRecord& record, const std::string& key) noexcept
    {
        std::string res;
        record.GetDataAsString(key, res);
        return res;
    }

    /**
        \brief Function for PrintContainerAsTable

        This function is used as an interface. It is called inside PrintContainerAsTable and allows it to work with VaulRecord and VaultRecordRef and their pointers.

        \param [in] recordRef the pointer to the VaultRecordRef
        \param [in] key the key whose value you want to get from the VaultRecordRef pointer

        \return the value of the key in the string
    */
    std::string GetDataAsString(VaultRecordRef* recordRef, const std::string& key) noexcept
    {
        std::string res;
        recordRef->GetDataAsString(key, res);
        return res;
    }

    /**
        \brief Function for PrintContainerAsTable

        This function is used as an interface. It is called inside PrintContainerAsTable and allows it to work with VaulRecord and VaultRecordRef and their pointers.

        \param [in] record the recordRef
        \param [in] key the key whose value you want to get from the VaultRecordRef

        \return the value of the key in the string
    */
    std::string GetDataAsString(const VaultRecordRef& recordRef, const std::string& key) noexcept
    {
        std::string res;
        recordRef.GetDataAsString(key, res);
        return res;
    }

    std::string GetUniqueId(VaultRecord* record) noexcept
    {
        std::stringstream ss;
        ss << record;
        return ss.str();
    }

    std::string GetUniqueId(const VaultRecord& record) noexcept
    {
        std::stringstream ss;
        ss << &record;
        return ss.str();
    }

    std::string GetUniqueId(VaultRecordRef* recordRef) noexcept
    {
        return recordRef->GetRecordUniqueId();
    }

    std::string GetUniqueId(const VaultRecordRef& recordRef) noexcept
    {
        return recordRef.GetRecordUniqueId();
    }
}