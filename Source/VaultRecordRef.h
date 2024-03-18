#pragma once

#include "VaultClasses.h"
#include "DataContainer.h"
#include "ReadWriteMutex.h"
#include "VaultRecord.h"
#include "Vault.h"
#include "VaultOperationResult.h"

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
        // Pointer to Vault
        Vault* Vlt = nullptr;

        // Pointer to VaultRecord inside Vault
        VaultRecord* DataRecord = nullptr;

        // This mutex is necessary because VaultRecucrsiveReadWriteMtx provides thread safety for Vault, but not for a specific VaultRecordRef object. 
        // For example, the GetRecord method or the comparison operator change the contents of VaultRecordRef, 
        // but there is no point in calling them to block Vault for writing
        mutable std::recursive_mutex Mtx;

        /**
            \brief The method for binding RecordRef to Record 
            
            \param [in] vaultRecord A pointer to the Vault referenced by the class object VaultRecordRef
            \param [in] vlt A pointer to Vault
        */
        void SetRecord(VaultRecord* vaultRecord, Vault* vlt);

    public:

        /// Making the Vault class friendly so that it has access to the internal members of the VaultRecordRef class
        friend Vault;

        /// Making the VaultRequestResult class friendly so that it has access to the internal members of the VaultRecordRef class
        friend VaultRequestResult;

        /// Making the std::hash<VaultRecordRef> struct friendly so that it has access to the internal members of the VaultRecordRef class
        friend std::hash<VaultRecordRef>;

        /// \todo Конструктор по умолчанию
        VaultRecordRef();
        
        /**
            \brief Constructor
            
            Simply call SetRecord method
            \param [in] vaultRecord A pointer to the VaultRecord referenced by the class object VaultRecordRef
            \param [in] vlt A pointer to Vault
        */
        VaultRecordRef(VaultRecord* vaultRecord, Vault* vlt);

        /// \brief Copy constructor
        /// \param [in] other other VaultRecordRef object
        VaultRecordRef(const VaultRecordRef& other);

        /// \brief Assignment operator
        /// \param [in] other other VaultRecordRef object
        VaultRecordRef& operator=(const VaultRecordRef& other);
        
        /// \brief Comparison operator
        /// \param [in] other the object to compare with
        /// \return true if the objects are equal, otherwise false. Two invalid objects are equal
        bool operator==(const VaultRecordRef& other) const;

        /// \brief A method for obtaining a unique record identifier
        ///  Important. Two VaultRecordRef objects pointing to the same record will return the same value. Invalid record will return null
        /// \return the unique identifier of the record
        std::string GetRecordUniqueId() const;

        /**
            \brief Method for updating data inside Vault

            \tparam <T> Any type of data except for c arrays

            Using this method, you can change the values inside the VaultRecord inside the Vault

            \param [in] key the key whose value needs to be changed
            \param [in] data new key data value

            \return VaultOperationResult object with GetData result.
        */
        template <class T>
        VaultOperationResult SetData(const std::string& key, const T& data);

        /**
            \brief Method for updating data inside Vault

            Using this method, you can change the values inside the VaultRecord inside the Vault.  
            See Vault::CreateRecord(std::vector<std::pair<std::string, DataSaver>> params) for more information

            \param [in] params a vector of pairs with data to be put in the Vault
            \todo Исправить
        */
        void SetData(const std::vector<std::pair<std::string, DataSaver>>& params);

        /**
            \brief A method for getting data using a key

            \tparam <T> Any type of data except for c arrays
            
            \param [in] key the key whose value should be obtained
            \param [in] data reference to record the received data

            \return VaultOperationResult object with GetData result.
        */
        template <class T>
        VaultOperationResult GetData(const std::string& key, T& data) const;
        
        /**
            \brief Method for getting data converted to string from a container using a key

            \param [in] key key for getting data
            \param [out] str a reference to string to write data from the container there.
            If the data was not found, then nothing will be written to the str

            It uses the ToString function inside. If it is not defined for the type, 
            then the str will be an empty string.

            \return Returns false if the key was not found, and otherwise returns true.
        */ 
        bool GetDataAsString(const std::string& key, std::string& str) const;

        /// \brief A function to check the validity of a class object
        /// An object may no longer be valid if the record it refers to has been deleted
        /// \return returns true if the object is valid, otherwise false
        bool IsValid() const;

        /// \brief The method for getting all the keys
        /// If the record is not valid, it will return an empty vector
        /// \return vector with keys
        std::vector<std::string> GetKeys() const;
        
        /// \brief A method for displaying VaultRecordRef content on the screen
        void PrintRecord() const;
        
        /// \brief A method for decoupling a class object from record. Reset class object to default state
        void Reset();

        /**
            \brief A method for locking record
            This method prevents the deletion of an entry from the Vault
            If the record is not valid, then the method will not do anything

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