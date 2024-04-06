#pragma once

#include "Vault.h"

namespace mvlt
{
    /** 
        \brief A class for storing query results
    
        The class inherits from Vault, but does not provide functionality for working with keys and creating new records.
        No new records will be created during the request, and this class will only store pointers to records in the original Vault.
    */
    class VaultRecordSet : protected Vault
    {
    private:

        // Pointer to parent Vault
        Vault* ParentVault = nullptr;

        // Variable to store parent vault state
        bool IsParentVaultValid = false;

        /**
            \brief The method for getting the result of the request

            \tparam <T> Any type of data except for c arrays

            \param [in] requestType Type of request
            \param [in] key the name of the key to search for
            \param [in] beginKeyValue the begin value of the key to be found
            \param [in] endKeyValue the end value of the key to be found
            \param [in] isIncludeBeginKeyValue include beginKeyValue in the interval or not
            \param [in] isIncludeEndKeyValue include endKeyValue in the interval or not
            \param [in] vaultRecordSet A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            
            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestRecords(const RequestType& requestType, const std::string& key, const T& beginKeyValue,
            const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue, 
            const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords) const;
    public:

        /// Make Vault class friend
        friend Vault;

        /// \brief Default constructor
        VaultRecordSet();

        /// \brief Copy constructor
        VaultRecordSet(const VaultRecordSet& other);

        /// \brief Operator assignment
        VaultRecordSet& operator=(const VaultRecordSet& other);
        
        /// \brief Method for checking the validity of the parent Vault
        /// \return true if it is valid, otherwise false
        bool GetIsParentVaultValid() const;

        /**
            \brief The method for getting a default key value

            \param [in] key the name of the key to search for

            \return returns false if the parent vault invalid or key was not found otherwise returns true
        */
        bool IsKeyExist(const std::string& key) const;

        /**
            \brief The method for getting a default key value

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] defaultKeyValue the value of the key

            \return returns false if the parent vault invalid or the key was not found otherwise returns true
        */
        template <class T>
        VaultOperationResult GetKeyValue(const std::string& key, T& defaultKeyValue) const;

        /**
            \brief The method for getting a key type

            \param [in] key the name of the key to search for
            \param [in] keyType the ref to std::type_index

            \return returns false if the parent vault invalid or the key was not found otherwise returns true
        */
        bool GetKeyType(const std::string& key, std::type_index& keyType) const;

        /// \brief Method for adding a record to VaultRecordSet
        /// \param [in] recordRef A constant reference pointing to a record
        void AddRecord(const VaultRecordRef& recordRef);

        /// \brief Method for adding a recordы to VaultRecordSet
        /// \param [in] recordRef A constant reference indicating a vector with refs
        void AddRecords(const std::vector<VaultRecordRef> recordsRefs);

        /**
            \brief The method for getting a reference to the data inside Vault

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordRef, where information about the requested record will be recorded.
            If the key is not found, or the saved type does not match the type T, the parameter will not change.
            If the key was found and the types matched, but the value was not found, 
            a nullptr record will be saved to the ref and the vaultRecordRef.isValid() method returns false.

            The method will return Ref to the original Vault, but if the original Vault and Vault Request are changed, the Result will be changed

            \return VaultOperationResult object with GetRecord result
        */
        template <class T>
        VaultOperationResult GetRecord(const std::string& key, const T& keyValue, VaultRecordRef& vaultRecordRef) const;

        /**
            \brief The method for getting a vector of references to the data inside Vault

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] recordsRefs A reference to std::vector<VaultRecordRef?, where information about the requested records will be recorded.
            In case of errors, the vector will not change
            \param [in] amountOfRecords The number of records requested
            
            \return VaultOperationResult object with GetRecords result
        */
        template <class T>
        VaultOperationResult GetRecords(const std::string& key, const T& keyValue, std::vector<VaultRecordRef>& recordsRefs, const std::size_t& amountOfRecords = -1) const;

        /**
            \brief A method for getting all records that have a value equal to keyValue stored by the key key

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            
            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords = -1) const;

        /**
            \brief A method for getting all records that have a value greater than keyValue stored by the key key

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            
            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestGreater(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords = -1) const;

        /**
            \brief A method for getting all records that have a value greater than or equal to keyValue stored by the key key

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            
            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestGreaterOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords = -1) const;

        /**
            \brief A method for getting all records that have a value less than keyValue stored by the key key

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            
            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestLess(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords = -1) const;

        /**
            \brief A method for getting all records that have a value less than or equal to keyValue stored by the key key

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            
            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestLessOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet, const std::size_t& amountOfRecords = -1) const;

        /**
            \brief The method for getting the result of the request

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] beginKeyValue the begin value of the key to be found
            \param [in] endKeyValue the end value of the key to be found
            \param [in] isIncludeBeginKeyValue include beginKeyValue in the interval or not
            \param [in] isIncludeEndKeyValue include endKeyValue in the interval or not
            \param [in] vaultRecordSet A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            
            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestInterval(const std::string& key, const T& beginKeyValue,
            const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue = true, 
            const bool& isIncludeEndKeyValue = true, const std::size_t& amountOfRecords = -1) const;
            
        /// \brief Resets the object to its initial state
        void Reset();

        /// \brief Clear VaultRecordSet
        /// Remove all references to records from the Vault Request Result, 
        /// but the records themselves in the original Vault will not be changed
        void Clear();

        /**
            \brief Method for remove a record from a Vault

            The record itself will not be changed in any way

            \param [in] recordRefToErase the reference to the record that needs to be removed

            \return Returns true if the record existed and was successfully removed, otherwise it returns false
        */
        bool RemoveRecord(const VaultRecordRef& recordRefToErase);

        /**
            \brief Method for remove a record from a Vault

            \tparam <T> Any type of data except for c arrays

            The record itself will not be changed in any way

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            
            \return VaultOperationResult object with RemoveRecord result
        */
        template <class T>
        VaultOperationResult RemoveRecord(const std::string& key, const T& keyValue);

        /**
            \brief The method for remove records using key and value

            \tparam <T> Any type of data except for c arrays

            The records itself will not be changed in any way

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] amountOfRecords The number of records to delete. By default set to minus one or all records.
            
            If the amountOfRecords is greater than the number of records stored inside the Vault, then all records with this key and value will be deleted.

            \return VaultOperationResult object with RemoveRecords result
        */
        template <class T>
        VaultOperationResult RemoveRecords(const std::string& key, const T& keyValue, const std::size_t& amountOfRecords = -1);

        /// \brief Method for getting the number of records
        /// If the parent Vault is not valid, it will return 0
        /// \return number of records
        std::size_t Size() const;

        /// \brief The method for getting all the keys
        /// If the parent Vault is not valid, it will return an empty vector
        /// \return vector with keys
        std::vector<std::string> GetKeys() const;

        /**
            \brief Method for getting sorted records

            \param [in] key The key by which the data should be sorted
            \param [in] isReverse Sort in descending order or descending order. By default, ascending
            \param [in] amountOfRecords The number of records. By default, everything is

            If the parent Vault is not valid, it will return an empty vector
            If the key is missing in the vault, the result vector will be empty
            \return A vector with links to records. The order of entries in the vector is determined by the amountOfRecords parameter
        */
        std::vector<VaultRecordRef> GetSortedRecords(const std::string& key, const bool& isReverse = false, const std::size_t& amountOfRecords = -1) const;

        /**
            \brief Method for handle sorted records

            \tparam <T> A function that takes const VaultRecordRef& as a parameter and returns bool.
            
            \param [in] key The key by which the data should be sorted
            \param [in] func A function takes const VaultRecordRef& as a parameter. If you need the function to be called again for the next record, 
            then this function call should return true. To stop the loop and interrupt the processing of sorted data, the function must return false. 
            To get values from a function, use lambdas and context capture
            \param [in] isReverse Sort in descending order or descending order. By default, ascending
            \param [in] amountOfRecords The number of records. By default, everything is

            If the parent Vault is not valid, it will call func 0 times
            The function iterate over all records sorted by the key parameter, in the order specified by the isReverse parameter. 
            For each record, the function passed in the func parameter is called.
            This function does not sort the data when it is called, the sorted data is already stored inside the Vault.
            If the key is missing in the vault, the function will be called 0 times
        */
        template<class F>
        void SortBy(const std::string& key, const F&& func, const bool& isReverse = false, const std::size_t& amountOfRecords = -1) const;

        /// \brief A method for displaying the contents of a Vault on the screen
        /// \param [in] amountOfRecords The number of records to be printed. The default value is -1, which means that all entries will be output
        void PrintVault(const std::size_t& amountOfRecords = -1) const;
        
        /**
            \brief A method for displaying the contents of a Vault as a table on the screen
            
            \param [in] isPrintId will the unique IDs be printed in the table
            \param [in] amountOfRecords The number of records to be printed. The default value is -1, which means that all entries will be output
            \param [in] keys vector of keys to be printed. By default, the vector is empty, which means that all keys will be output
        */
        void PrintAsTable(bool isPrintId = false, const std::size_t& amountOfRecords = -1, const std::vector<std::string> keys = {}) const;

        /// \brief Destructor
        ~VaultRecordSet();
    };
}