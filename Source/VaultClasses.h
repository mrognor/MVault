#pragma once

#include "DataContainer.h"

namespace mvlt
{
    class Vault;
    class VaultRecord;
    class VaultRecordRef;
    class VaultRequestResult;
    
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