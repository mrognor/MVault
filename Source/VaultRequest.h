#pragma once

#include <functional>

#include "VaultClasses.h"

namespace mvlt
{
    /// \brief A class for requests to Vault
    /// \tparam <VaultRequestType> type of the request
    template <VaultRequestType Type>
    class VaultRequest 
    {
    protected:
        /// A variable that stores a function that requests data from Vault and stores it in VaultRecordSet
        std::function<void (Vault*, VaultRecordSet&)> RequestFunc;

    public:

        /**
            \brief Request constructor

            \tparam <T> Any type of data except c-arrays

            \param [in] key requested key
            \param [in] keyValue requested data
        */
        template <class T>
        VaultRequest(const std::string& key, const T& keyValue);

        /**
            \brief Function for requesting data from Vault

            \param [in] vlt A pointer to the Vault from which to request data
            \param [in] vaultRecordSet A reference to the Vault RecordSet where the recordings will be saved
        */
        void Request(Vault* vlt, VaultRecordSet& vaultRecordSet) const;
    };
}