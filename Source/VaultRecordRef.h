#pragma once

#include "VaultClasses.h"
#include "DataContainer.h"
#include "ReadWriteMutex.h"
#include "VaultRecord.h"

namespace mvlt 
{
/**
        \brief A class that provides access to data inside Vault.

        This class works as a reference to the data inside the Vault, you can use it to change the data inside the Vault.
        If two objects of the class point to the same VaultRecord inside the Vault, 
        then when the data in one object changes, the data in the other object will also change.
        If the record pointed to by an object of this class is deleted, the isValid function will return false, 
        and any attempt to access or change the data will be ignored.
    */
    class VaultRecordRef
    {
    private:
        // Pointer to VaultRecord inside Vault
        VaultRecord* DataRecord = nullptr;

        // Pointer to VaultStructureHashMap 
        VaultStructureHashMap* VaultHashMapStructure = nullptr;

        // Pointer to VaultStructureMap 
        VaultStructureMap* VaultMapStructure = nullptr;

        // Pointer to recursive read write mutex inside data storage
        RecursiveReadWriteMutex* VaultRecucrsiveReadWriteMtx = nullptr;
    public:

        /// Making the Vault class friendly so that it has access to the internal members of the VaultRecordRef class
        friend Vault;

        /// Making the std::hash<VaultRecordRef> struct friendly so that it has access to the internal members of the VaultRecordRef class
        friend std::hash<VaultRecordRef>;

        /**
            \brief Constructor
            
            \param [in] vaultRecord A pointer to the Vault referenced by the class object VaultRecordRef
            \param [in] vaultStructureHashMap The internal structure of the Vault, represented by a hash table
            \param [in] vaultStructureMap The internal structure of the Vault, represented by a binary tree
            \param [in] vaultRecucrsiveReadWriteMtx Pointer to Vault mutex used for thread safety
        */
        VaultRecordRef(VaultRecord* vaultRecord, 
            VaultStructureHashMap* vaultStructureHashMap, 
            VaultStructureMap* vaultStructureMap,
            RecursiveReadWriteMutex* vaultRecucrsiveReadWriteMtx);

        /// \brief Copy constructor
        /// \param [in] other other VaultRecordRef object
        VaultRecordRef(const VaultRecordRef& other);

        /// \brief Assignment operator
        /// \param [in] other other VaultRecordRef object
        VaultRecordRef& operator=(const VaultRecordRef& other);
        
        /// \brief Comparison operator
        /// \param [in] other the object to compare with
        /// \return true if the objects are equal, otherwise false
        bool operator==(const VaultRecordRef& other) const;

        /// \brief A method for obtaining a unique record identifier
        ///  Important. Two VaultRecordRef objects pointing to the same record will return the same value
        /// \return the unique identifier of the record
        std::string GetRecordUniqueId() const;

        /**
            \brief Method for updating data inside Vault

            \tparam <T> Any type of data except for c arrays

            Using this method, you can change the values inside the VaultRecord inside the Vault

            \param [in] key the key whose value needs to be changed
            \param [in] data new key data value

            \return returns true if the key was found otherwise returns false
        */
        template <class T>
        bool SetData(const std::string& key, const T& data)
        {
            // A pointer for storing a std::unordered_multimap in which a template data type is used as a key, 
            // and a pointer to the DataHashMap is used as a value
            std::unordered_multimap<T, VaultRecord*>* TtoVaultRecordHashMap = nullptr;

            // A pointer for storing a std::multimap in which a template data type is used as a key, 
            // and a pointer to the DataHashMap is used as a value
            std::multimap<T, VaultRecord*>* TtoVaultRecordMap = nullptr;

            // Lock Vault to write
            VaultRecucrsiveReadWriteMtx->WriteLock();

            // Get std::unordered_multimap with T key and VaultRecord* value
            if (!VaultHashMapStructure->GetData(key, TtoVaultRecordHashMap)) 
            {
                VaultRecucrsiveReadWriteMtx->WriteUnlock();
                return false;
            }

            // Get std::multimap with T key and VaultRecord* value
            if (!VaultMapStructure->GetData(key, TtoVaultRecordMap))
            {
                VaultRecucrsiveReadWriteMtx->WriteUnlock();
                return false;
            }

            if (DataRecord == nullptr || !DataRecord->GetIsValid())
            {
                VaultRecucrsiveReadWriteMtx->WriteUnlock();
                return false;
            }

            // Get the current value of the key key inside the VaultRecordRef and save it for further work
            T oldData;
            DataRecord->GetData(key, oldData);

            // Remove oldData from TtoVaultRecordHashMap from VaultHashMapStructure
            auto FirstAndLastIteratorsWithKeyOnHashMap = TtoVaultRecordHashMap->equal_range(oldData);

            // Iterate over all data records with oldData key
            for (auto& it = FirstAndLastIteratorsWithKeyOnHashMap.first; it != FirstAndLastIteratorsWithKeyOnHashMap.second; ++it)
            {
                // Find required data record
                if (it->second == DataRecord)
                {
                    TtoVaultRecordHashMap->erase(it);
                    break;
                }
            }

            // Add new data to TtoVaultRecordHashMap to Vault VaultHashMapStructure
            TtoVaultRecordHashMap->emplace(data, DataRecord);

            // Remove oldData from TtoVaultRecordMap from VaultMapStructure
            auto FirstAndLastIteratorsWithKeyOnMap = TtoVaultRecordMap->equal_range(oldData);

            // Iterate over all data records with oldData key
            for (auto& it = FirstAndLastIteratorsWithKeyOnMap.first; it != FirstAndLastIteratorsWithKeyOnMap.second; ++it)
            {
                // Find required data record
                if (it->second == DataRecord)
                {
                    TtoVaultRecordMap->erase(it);
                    break;
                }
            }

            // Add new data to TtoVaultRecordMap to Vault VaultMapStructure
            TtoVaultRecordMap->emplace(data, DataRecord);

            // Update data inside VaultRecord pointer inside VaultRecordRef and Vault
            DataRecord->SetData(key, data);

            // Unlock Vault
            VaultRecucrsiveReadWriteMtx->WriteUnlock();
            return true;
        }

        /**
            \brief Method for updating data inside Vault

            Using this method, you can change the values inside the VaultRecord inside the Vault.  
            See Vault::CreateRecord(std::vector<std::pair<std::string, DataSaver>> params) for more information

            \param [in] params a vector of pairs with data to be put in the Vault
        */
        void SetData(const std::vector<std::pair<std::string, DataSaver>>& params);

        /**
            \brief A method for getting data using a key

            \tparam <T> Any type of data except for c arrays
            
            \param [in] key the key whose value should be obtained
            \param [in] data reference to record the received data

            \return returns true if the data was received, otherwise false
        */
        template <class T>
        bool GetData(const std::string& key, T& data) const
        {
            bool res;
            VaultRecucrsiveReadWriteMtx->ReadLock();
            if (DataRecord->GetIsValid()) res = DataRecord->GetData(key, data);
            VaultRecucrsiveReadWriteMtx->ReadUnlock();
            return res;
        }

        /// \brief A function to check the validity of a class object
        /// An object may no longer be valid if the record it refers to has been deleted
        /// \return returns true if the object is valid, otherwise false
        bool IsValid() const;

        /// \brief A method for displaying VaultRecordRef content on the screen
        void PrintRecord() const;
        
        /// A method for decoupling a class object from record
        /// \todo Нужен ли этот метод
        void Unlink();

        /**
            \brief A method for locking record

            Usage example:
            \code{.cpp}
            Vault vlt;
            vlt.SetKey<std::size_t>("Id", 0);
            vlt.SetKey<std::vector<int>>("Friends", std::vector<int>());

            ...

            std::vector<int> friends;
            VaultRecordRef vltrr = vlt.GetRecord("Friends", friends);

            // Lock so that another thread cannot change the vector
            vltrr.ReadLock();

            for (const int& friend : friends)
                std::cout << friend << std::endl;

            vltrr.ReadUnlock();
            
            \endcode
        */
        void ReadLock() const;

        /// \brief A method for unlocking record
        void ReadUnlock() const;

        /// \brief Default destructor
        ~VaultRecordRef();
    };
}