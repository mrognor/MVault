#pragma once

#include "Vault.h"
#include "VaultRecord.h"
#include "VaultClasses.h"
#include "VaultParamInput.h"
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
        VaultRecord* VaultRecordPtr = nullptr;

        /**
            \brief The method for binding RecordRef to Record

            \param [in] vaultRecord A pointer to the Vault referenced by the class object VaultRecordRef
            \param [in] vlt A pointer to Vault
        */
        void SetRecord(VaultRecord* vaultRecord, Vault* vlt) noexcept;

        /**
            \brief Constructor

            Simply call SetRecord method
            \param [in] vaultRecord A pointer to the VaultRecord referenced by the class object VaultRecordRef
            \param [in] vlt A pointer to Vault
        */
        VaultRecordRef(VaultRecord* vaultRecord, Vault* vlt) noexcept;
    public:
        /// Making the Vault class friendly so that it has access to the internal members of the VaultRecordRef class
        friend Vault;

        /// Making the VaultRecordSet class friendly so that it has access to the internal members of the VaultRecordRef class
        friend VaultRecordSet;

        /// Making the std::hash<VaultRecordRef> struct friendly so that it has access to the internal members of the VaultRecordRef class
        friend std::hash<VaultRecordRef>;

        /// \brief Default constructor
        VaultRecordRef() noexcept;

        /**
            \brief Copy constructor

            \param [in] other other VaultRecordRef object
        */
        VaultRecordRef(const VaultRecordRef& other) noexcept;

        /**
            \brief Move constructor

            \param [in] other other VaultRecordRef object
        */
        VaultRecordRef(VaultRecordRef&& other) noexcept;

        /**
            \brief Assignment operator

            \param [in] other other VaultRecordRef object
        */
        VaultRecordRef& operator=(const VaultRecordRef& other) noexcept;

        /**
            \brief Move assignment operator

            \param [in] other other VaultRecordRef object
            \return returns a new object, with data from other
        */
        VaultRecordRef& operator=(VaultRecordRef&& other) noexcept;

        /**
            \brief Comparison operator

            \param [in] other the object to compare with

            \return true if the objects are equal, otherwise false
        */
        bool operator==(const VaultRecordRef& other) const noexcept;

        /**
            \brief Not equation operator

            \param [in] other the object to compare with

            \return true if the objects are not equal, otherwise true
        */
        bool operator!=(const VaultRecordRef& other) const noexcept;

        /**
            \brief A method for obtaining a unique record identifier

            Important. Two VaultRecordRef objects pointing to the same record will return the same value. Invalid record will return null

            \return the unique identifier of the record
        */
        std::string GetRecordUniqueId() const noexcept;

        /**
            \brief Method for updating data inside Vault

            \tparam <T> Any type of data except for c arrays

            Using this method, you can change the values inside the VaultRecord inside the Vault

            \param [in] key the key whose value needs to be changed
            \param [in] data new key data value

            \return VaultOperationResult object with GetData result.
        */
        template <class T>
        VaultOperationResult SetData(const std::string& key, const T& data) noexcept;

        /**
            \brief Method for updating data inside Vault

            Using this method, you can change the values inside the VaultRecord inside the Vault.
            See Vault::CreateRecord(std::vector<std::pair<std::string, VaultParamInput>>&& params) for more information

            \param [in] params a vector of pairs with data to be put in the Vault

            \return If one of the parameters is incorrect, it returns an error when setting this parameter, and all subsequent parameters will not be set
        */
        VaultOperationResult SetData(const std::vector<std::pair<std::string, VaultParamInput>>& params) noexcept;

        /**
            \brief A method for getting data using a key

            \tparam <T> Any type of data except for c arrays

            \param [in] key the key whose value should be obtained
            \param [in] data reference to record the received data

            \return VaultOperationResult object with GetData result.
        */
        template <class T>
        VaultOperationResult GetData(const std::string& key, T& data) const noexcept;

        /**
            \brief Method for getting data converted to string from a container using a key

            \param [in] key key for getting data
            \param [out] str a reference to string to write data from the container there.
            If the data was not found, then nothing will be written to the str

            It uses the ToString function inside. If it is not defined for the type,
            then the str will be an empty string.

            \return VaultOperationResult object with GetDataAsString result.
        */
        VaultOperationResult GetDataAsString(const std::string& key, std::string& str) const noexcept;

        /**
            \brief A function to check the validity of a class object

            An object may no longer be valid if the record it refers to has been deleted

            \return returns true if the object is valid, otherwise false
        */
        bool IsValid() const noexcept;

        /**
            \brief The method for getting a default key value

            \param [in] key the name of the key to search for

            \return returns true if the key was found otherwise returns false
        */
        bool IsKeyExist(const std::string& key) const noexcept;

        /**
            \brief The method for getting all the keys

            If the record is not valid, it will return an empty vector

            \return vector with keys
        */
        std::vector<std::string> GetKeys() const noexcept;

        /// \brief A method for displaying VaultRecordRef content on the screen
        void PrintRecord() const noexcept;

        /// \brief A method for decoupling a class object from record. Reset class object to default state
        void Reset() noexcept;

        /**
            \brief Method for convert ref to json

            \param [in] isFormat is format json message
            \param [in] tabSize size of tab

            \return json string with data from ref
        */
        std::string ToJson(const bool& isFormat = true, const std::size_t& tabSize = 2) const noexcept;

        /**
            \brief Method for convert vault record ref to strings

            \return vector with record poles. Each pole is a key and value string pair
        */
        std::vector<std::pair<std::string, std::string>> ToStrings() const noexcept;

        /// \brief Default destructor
        ~VaultRecordRef() noexcept;
    };
}