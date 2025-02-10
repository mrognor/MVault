#pragma once

#include <functional>

#include "VaultClasses.h"

namespace mvlt
{
    /**
        \brief A class for requests to Vault
        
        \tparam <VaultRequestType> type of the request
    */
    template <VaultRequestType Type>
    class VaultRequest 
    {
    private:

        /// Request type
        VaultRequestType RequestType = Type;

        /// Function for requesting data
        void (*DataRequestFunc)(const std::string&, Vault*, std::unordered_set<VaultRecord*>&, void*, void*, bool, bool, std::function<bool(const VaultRecordRef&)>);
        
        /// A variable for storing a logical request function
        std::function<void (Vault*, std::unordered_set<VaultRecord*>&)> LogicalRequestFunction;

        /// A pointer to a function that delete data
        void (*DeleteFunc)(void*) = nullptr;

        /// Request key
        std::string Key;

        /// Pointer to Data saved inside Request
        void* DataPtr = nullptr;

        /// Variable to store predicat function
        std::function<bool(const VaultRecordRef&)> RequestPredicat;

    public:

        /// Making the all VaultRequest templates friendly
        template <VaultRequestType TType>
        friend class VaultRequest;

        /// \brief Default construcor
        VaultRequest() noexcept = default;

        /**
            \brief Copy constructor

            \param [in] other object to copy
        */
        VaultRequest(const VaultRequest<Type>& other) noexcept = default;

        /**
            \brief Move constructor

            \param [in] other object to move
        */
        VaultRequest(VaultRequest<Type>&& other) noexcept = default;

        /**
            \brief Assignment operator

            \param [in] other object to copy
        */
        VaultRequest& operator=(const VaultRequest<Type>& other) noexcept = default;
        /**
            \brief Move assignment operator

            \param [in] other object to move
        */
        VaultRequest& operator=(VaultRequest<Type>&& other) noexcept = default;

        /**
            \brief Request constructor

            \tparam <T> Any type of data except c-arrays

            \param [in] key requested key
            \param [in] keyValue requested data
            \param [in] requestPredicat A function that accepts VaultRecordRef and decides whether to add an record to the request. 
            When the function returns true, the record is added, and when false is not added.
        */
        template <class T>
        VaultRequest(const std::string& key, const T& keyValue, const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat);

        /**
            \brief Request constructor

            \tparam <RequestType1> First request type
            \tparam <RequestType2> Second request type

            \param [in] request1 first request
            \param [in] request2 second request
            \param [in] requestPredicat A function that accepts VaultRecordRef and decides whether to add an record to the request. 
            When the function returns true, the record is added, and when false is not added.
        */
        template <VaultRequestType RequestType1, VaultRequestType RequestType2>
        VaultRequest(const VaultRequest<RequestType1>& request1, const VaultRequest<RequestType2>& request2) noexcept;

        /**
            \brief Function for requesting data from Vault

            \param [in] vlt A pointer to the Vault from which to request data
            \param [in] vaultRecords A reference to the std::unordered_set<VaultRecord*> where the recordings will be saved
        */
        void Request(Vault* vlt, std::unordered_set<VaultRecord*>& vaultRecords) const;

        /// \brief Default destructor
        ~VaultRequest() noexcept;

        /// \brief Friend operator&&
        template <VaultRequestType TType>
        friend VaultRequest operator&& (const VaultRequest& lhs, const VaultRequest& rhs) noexcept;

        /// \brief Friend operator||
        template <VaultRequestType TType>
        friend VaultRequest operator|| (const VaultRequest& lhs, const VaultRequest& rhs) noexcept;
    };

    /**
        \brief Operator&& for requests

        \tparam <LType> Specialization for a specific value with type VaultRequestType in lhs record 
        \tparam <RType> Specialization for a specific value with type VaultRequestType in rhs record 

        Allows you to simply use && instead of calling And.
        
        Instead of writing:

        \code{.cpp}
        vlt.Request(mvlt::And(mvlt::Less("A", 3), mvlt::Greater("A", 7)), vrs1);
        \endcode

        You can write like this:

        \code{.cpp}
        vlt.Request(mvlt::Less("A", 3) && mvlt::Greater("A", 7), vrs1);
        \endcode

        \param [in] lhs left request value
        \param [in] rhs right request value

        \return new request equal to VaultRequest<VaultRequestType::And>
    */
    template <VaultRequestType LType, VaultRequestType RType>
    VaultRequest<VaultRequestType::And> operator&& (const VaultRequest<LType>& lhs, const VaultRequest<RType>& rhs) noexcept;

    /**
        \brief Operator|| for requests

        \tparam <LType> Specialization for a specific value with type VaultRequestType in lhs record 
        \tparam <RType> Specialization for a specific value with type VaultRequestType in rhs record 

        Allows you to simply use && instead of calling Or.
        
        Instead of writing:

        \code{.cpp}
        vlt.Request(mvlt::Or(mvlt::Less("A", 3), mvlt::Greater("A", 7)), vrs1);
        \endcode

        You can write like this:

        \code{.cpp}
        vlt.Request(mvlt::Less("A", 3) || mvlt::Greater("A", 7), vrs1);
        \endcode

        \param [in] lhs left request value
        \param [in] rhs right request value

        \return new request equal to VaultRequest<VaultRequestType::Or>
    */
    template <VaultRequestType LType, VaultRequestType RType>
    VaultRequest<VaultRequestType::Or> operator|| (const VaultRequest<LType>& lhs, const VaultRequest<RType>& rhs) noexcept;
}