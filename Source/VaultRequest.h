#pragma once

#include <functional>

#include "VaultClasses.h"

namespace mvlt
{
    /// A class for requests to Vault
    class VaultRequest 
    {
    protected:
        /// A variable that stores a function that requests data from Vault and stores it in VaultRecordSet
        std::function<void (Vault*, VaultRecordSet&)> RequestFunc;

    public:
        /**
            \brief Function for requesting data from Vault

            \param [in] vlt A pointer to the Vault from which to request data
            \param [in] vaultRecordSet A reference to the Vault RecordSet where the recordings will be saved
        */
        void Request(Vault* vlt, VaultRecordSet& vaultRecordSet) const;
    };

    /// A class for requests equal records from Vault
    class Equal : public VaultRequest 
    {
    public:
        /**
            \brief Constructor with parameters

            \tparam <T> Any type of data except for c arrays

            \param [in] key the requested key
            \param [in] keyValue the requested key value
        */
        template<class T>
        Equal(const std::string& key, const T& keyValue);
    };

    /// A class for requests greater records from Vault
    class Greater : public VaultRequest 
    {
    public:
        /**
            \brief Constructor with parameters

            \tparam <T> Any type of data except for c arrays

            \param [in] key the requested key
            \param [in] keyValue the requested key value
        */
        template<class T>
        Greater(const std::string& key, const T& keyValue);
    };

    /// A class for requests greater or equal records from Vault
    class GreaterOrEqual : public VaultRequest 
    {
    public:
        /**
            \brief Constructor with parameters

            \tparam <T> Any type of data except for c arrays

            \param [in] key the requested key
            \param [in] keyValue the requested key value
        */
        template<class T>
        GreaterOrEqual(const std::string& key, const T& keyValue);
    };

    /// A class for requests less records from Vault
    class Less : public VaultRequest 
    {
    public:
        /**
            \brief Constructor with parameters

            \tparam <T> Any type of data except for c arrays

            \param [in] key the requested key
            \param [in] keyValue the requested key value
        */
        template<class T>
        Less(const std::string& key, const T& keyValue);
    };

    /// A class for requests less or equal records from Vault
    class LessOrEqual : public VaultRequest 
    {
    public:
        /**
            \brief Constructor with parameters

            \tparam <T> Any type of data except for c arrays

            \param [in] key the requested key
            \param [in] keyValue the requested key value
        */
        template<class T>
        LessOrEqual(const std::string& key, const T& keyValue);
    };
}