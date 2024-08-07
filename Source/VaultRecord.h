#pragma once

#include <vector>
#include <sstream>
#include <mutex>

#include "VaultClasses.h"

namespace mvlt
{
    /**
        \brief A class for storing data inside Vault

        It is a wrapper over the Data Hash Map, but adds a functionality to invalidate VaultRecordRef's pointing to an object of this class.  
        The functionality from HashMap stores allows you to store data of any type and provide access to them using string keys.  
    */ 
    class VaultRecord : public DataHashMap
    {
    private:
        // Variable to store object validity
        bool IsValid = true;

        // Validity counter
        std::size_t RefCounter = 0;

        // Mutex for thread safety
        mutable std::recursive_mutex VaultRecordMutex;

        // Set with all dependent VaultRecordSets
        std::unordered_set<VaultRecordSet*> dependentVaultRecordSets;

    public:

        /// Making the Vault class friendly so that it has access to the internal members of the VaultRecord class
        friend Vault;
        
        /// Making the VaultRecordRef class friendly so that it has access to the internal members of the VaultRecord class
        friend VaultRecordRef;

        /// Making the VaultRecordSet class friendly so that it has access to the internal members of the VaultRecord class
        friend VaultRecordSet;
        
        /// \brief Default constructor
        VaultRecord() noexcept;

        /// \brief Copy constructor
        VaultRecord(const VaultRecord& other) noexcept;

        /// \brief A method for increasing the number of references to an object
        void AddRef() noexcept;

        /// \brief A method to reduce the number of references to an object
        void RemoveRef() noexcept;

        /// \brief A method to indicate that the record is no longer valid and is not inside Vault
        void Invalidate() noexcept;

        /**
            \brief A method for checking whether a record is inside a Vault
            \return returns the validity of the record
        */
        bool GetIsValid() noexcept;
    };
}