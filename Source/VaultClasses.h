#pragma once

#include <functional>

#include "DataSaver.h"
#include "DataContainer.h"
#include "ReadWriteMutex.h"

namespace mvlt
{
    /// \brief An enum with the types of all possible requests
    enum class VaultRequestType 
    {
        Greater,       ///< A request that will receive all records with a key value greater than the requested one
        GreaterOrEqual,///< A request that will receive all records with a key value greater than or equal to the requested one
        Equal,         ///< A request that will receive all records whose key value is equal to the requested one
        LessOrEqual,   ///< A request that will receive all records whose key value is less than or equal to the requested one
        Less,          ///< A request that will receive all records with a key value less than the requested one
        Interval,      ///< A request that will receive all records with a key value between requested
        Or,
        And
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

    /// \brief The default request predicate function
    /// The function accepts VaultRecordRef and returns true. It is needed as a stub for the default parameter in requests. It is never called
    extern std::function<bool(const VaultRecordRef&)> DefaultRequestPredicat;
    
    /**
        A simple typedef for HashMap. It is necessary for a more understandable separation of types.
        Represents the internal structure of the Vault.
        A string with the name of the key is used as the key. All keys are the same as in Vault.
        The value stores a pointer to std::unordered_multimap<T, VaultRecord*>.
        The key type is same as the Vault key value type.
        The value is a pointer to VaultRecord.

        Such a complex structure is needed to quickly, in O(1), search for each key with any type.
    */
    typedef DataHashMap VaultStructureHashMap;

    /**
        A simple typedef for Map. It is necessary for a more understandable separation of types.
        Represents the internal structure of the Vault.
        A string with the name of the key is used as the key. All keys are the same as in Vault.
        The value stores a pointer to std::multimap<T, VaultRecord*>.
        The key type is same as the Vault key value type.
        The value is a pointer to VaultRecord.

        Such a complex structure is necessary in order to quickly, in O(log n), 
        find a set of elements that meet a certain requirement, for example, more than a certain value or less than this value
    */
    typedef DataMap VaultStructureMap;
}