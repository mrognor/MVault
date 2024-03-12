#pragma once

#include <vector>
#include <sstream>
#include <mutex>

#include "VaultClasses.h"
#include "DataContainer.h"
#include "ReadWriteMutex.h"

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
        std::mutex* Mtx = nullptr;

        /// \todo
        Vault* ParentVault = nullptr;

        /// \todo
        VaultRecord* ParentVaultRecord = nullptr;

        /// \todo
        friend Vault;
        friend VaultRecordRef;

    public:
        /// \brief Default constructor
        VaultRecord();

        /// \brief Copy constructor
        VaultRecord(const VaultRecord& other);

        /// \brief A method for increasing the number of references to an object
        void AddRef();

        /// \brief A method to reduce the number of references to an object
        void RemoveRef();

        /// \brief A method to indicate that the record is no longer valid and is not inside Vault
        void Invalidate();

        /// \brief A method for checking whether a record is inside a Vault
        /// \return returns the validity of the record
        bool GetIsValid();

        /// \brief Default destructor
        ~VaultRecord();
    };
}