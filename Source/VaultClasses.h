#pragma once

#include <functional>

#include "DataSaver.h"
#include "DataContainer.h"
#include "ReadWriteMutex.h"

namespace mvlt
{
    /// \brief An enum with the types of all possible requests
    enum class VaultRequestType : std::uint8_t
    {
        Greater,       ///< A request that will receive all records with a key value greater than the requested one
        GreaterOrEqual,///< A request that will receive all records with a key value greater than or equal to the requested one
        Equal,         ///< A request that will receive all records whose key value is equal to the requested one
        LessOrEqual,   ///< A request that will receive all records whose key value is less than or equal to the requested one
        Less,          ///< A request that will receive all records with a key value less than the requested one
        Interval,      ///< A request that will receive all records with a key value between requested
        Or,            ///< A request with a logical operation or
        And            ///< A request with a logical operation and
    };

    // MVault classes
    class Vault;
    class VaultRecord;
    class VaultRecordRef;
    class VaultRecordSet;
    class VaultParamInput;
    
    // MVault structs
    struct VaultOperationResult;
    
    // MVault requests
    template <VaultRequestType Type>
    class VaultRequest;

    typedef VaultRequest<VaultRequestType::Less> Less;
    typedef VaultRequest<VaultRequestType::LessOrEqual> LessOrEqual;
    typedef VaultRequest<VaultRequestType::Equal> Equal;
    typedef VaultRequest<VaultRequestType::GreaterOrEqual> GreaterOrEqual;
    typedef VaultRequest<VaultRequestType::Greater> Greater;
    typedef VaultRequest<VaultRequestType::Or> Or;
    typedef VaultRequest<VaultRequestType::And> And;

    /**
        \brief The default request predicate function
    
        The function accepts VaultRecordRef and returns true. It is needed as a stub for the default parameter in requests. It is never called
    */
    extern std::function<bool(const VaultRecordRef&)> DefaultRequestPredicat;
}