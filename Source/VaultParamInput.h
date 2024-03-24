#pragma once

#include <string>
#include <typeindex>
#include <functional>

#include "VaultClasses.h"
#include "VaultRecordRef.h"

namespace mvlt
{
    /// \brief A class for transferring data to the Vault
    /// It is used for multiple data additions when creating a record in Vault and for multiple data changes when setting data in VaultRecordRef
    class VaultParamInput
    {
    private:

        // Pointer to data type saver
        std::type_index DataType;
        
        // Function for setting data to VaultRecordRef
        std::function<VaultOperationResult(const std::string&, VaultRecordRef&)> SetDataToRefFunc;
        
        // Function for settind data to VaultRecord
        std::function<void(const std::string&, VaultRecord*)> SetDataToRecordFunc;
        
        /**
            \brief Method for setting key data in VaultRecordRef

            \param [in] key data key
            \param [in] refToSetData ref to set data

            \return VaultRecordRef::SetData status
        */
        VaultOperationResult SetDataToRef(const std::string& key, VaultRecordRef& refToSetData) const;

        /// \brief Method for setting key data in VaultRecord
        /// \param [in] key data key
        /// \param [in] recordToSetData pointer to VaultRecord
        void SetDataToRecord(const std::string& key, VaultRecord* recordToSetData) const;

    public:
        
        /// Making the Vault class friendly so that it has access to the internal members of the VaultParamInput class
        friend Vault;

        /// Making the VaultRecordRef class friendly so that it has access to the internal members of the VaultParamInput class
        friend VaultRecordRef;
        
        /// \brief A template constructor that accepts a variable to store inside VaultParamInput
        /// \tparam <T> Any type of data except for c arrays
        /// \param [in] data data to be stored inside VaultParamInput
        template <class T>
        VaultParamInput(const T& data);

        /// \brief A method for getting the type of saved data.
        /// \return std::type_index object with saved data type
        std::type_index GetDataType() const;
    };
}