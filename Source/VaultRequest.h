#pragma once

#include <functional>

#include "VaultClasses.h"

namespace mvlt
{
    /// A class for requests to Vault
    template <VaultRequestType Type>
    class VaultRequest 
    {
    protected:
        /// A variable that stores a function that requests data from Vault and stores it in VaultRecordSet
        std::function<void (Vault*, VaultRecordSet&)> RequestFunc;

    public:

        template <class T>
        VaultRequest(const std::string& key, const T& keyValue);

        /**
            \brief Function for requesting data from Vault

            \param [in] vlt A pointer to the Vault from which to request data
            \param [in] vaultRecordSet A reference to the Vault RecordSet where the recordings will be saved
        */
        void Request(Vault* vlt, VaultRecordSet& vaultRecordSet) const;
    };
    
    template <>
    class VaultRequest <VaultRequestType::Equal> {};

    template <>
    class VaultRequest <VaultRequestType::Greater> {};

    template <>
    class VaultRequest <VaultRequestType::GreaterOrEqual> {};

    template <>
    class VaultRequest <VaultRequestType::Less> {};

    template <>
    class VaultRequest <VaultRequestType::LessOrEqual> {};
}