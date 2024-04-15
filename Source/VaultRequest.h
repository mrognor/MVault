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
    private:

        /// Request type
        VaultRequestType RequestType = Type;

        /// Function for requesting data
        void (*DataRequestFunc)(const std::string&, Vault*, std::unordered_set<VaultRecord*>&, void*, void*, bool, bool);
        
        /// A variable for storing a logical request function
        std::function<void (Vault*, std::unordered_set<VaultRecord*>&)> LogicalRequestFunction;

        /// A pointer to a function that delete data
        void (*DeleteFunc)(void*) = nullptr;

        /// Request key
        std::string Key;

        /// Pointer to Data saved inside Request
        void* DataPtr = nullptr;

    public:

        /// Making the all VaultRequest templates friendly
        template <VaultRequestType TType>
        friend class VaultRequest;

        /**
            \brief Request constructor

            \tparam <T> Any type of data except c-arrays

            \param [in] key requested key
            \param [in] keyValue requested data
        */
        template <class T>
        VaultRequest(const std::string& key, const T& keyValue);

        /**
            \brief Request constructor

            \tparam <RequestType1> First request type
            \tparam <RequestType2> Second request type

            \param [in] request1 first request
            \param [in] request2 second request
        */
        template <VaultRequestType RequestType1, VaultRequestType RequestType2>
        VaultRequest(const VaultRequest<RequestType1>& request1, const VaultRequest<RequestType2>& request2);

        /**
            \brief Function for requesting data from Vault

            \param [in] vlt A pointer to the Vault from which to request data
            \param [in] vaultRecords A reference to the std::unordered_set<VaultRecord*> where the recordings will be saved
        */
        void Request(Vault* vlt, std::unordered_set<VaultRecord*>& vaultRecords) const;

        /// \brief Default destructor
        ~VaultRequest();
    };
}