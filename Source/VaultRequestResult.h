#pragma once

#include "Vault.h"

namespace mvlt
{
    /** 
        \brief A class for storing query results
    
        The class inherits from Vault, but does not provide functionality for working with keys and creating new records.
        No new records will be created during the request, and this class will only store pointers to records in the original Vault.
    */
    class VaultRequestResult : protected Vault
    {
    private:

        // Pointer to parent Vault
        Vault* ParentVault = nullptr;

        // Variable to store parent vault state
        bool IsParentVaultValid = false;

    public:

        /// Make Vault class frien
        friend Vault;

        /// \brief Default constructor
        VaultRequestResult();

        /// \brief Copy constructor
        VaultRequestResult(const VaultRequestResult& other);

        /// \brief Operator assignment
        VaultRequestResult& operator=(const VaultRequestResult& other);
        
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
        VaultOperationResult GetKeyValue(const std::string& key, T& defaultKeyValue) const
        {
            VaultOperationResult res;
            RecursiveReadWriteMtx.ReadLock();

            // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
            if (IsParentVaultValid)
            {
                ParentVault->RecursiveReadWriteMtx.ReadLock();
                res = Vault::GetKeyValue(key, defaultKeyValue);
                ParentVault->RecursiveReadWriteMtx.ReadUnlock();
            }
            else 
            {
                res.Key = key;
                res.RequestedType = typeid(defaultKeyValue);
                res.IsOperationSuccess = false;
                res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
            }
            RecursiveReadWriteMtx.ReadUnlock();
            return res;
        }

        /**
            \brief The method for getting a key type

            \param [in] key the name of the key to search for
            \param [in] keyType the ref to std::type_index

            \return returns false if the parent vault invalid or the key was not found otherwise returns true
        */
        bool GetKeyType(const std::string& key, std::type_index& keyType) const;

        /// \brief Method for adding a record to VaultRequestResult
        /// \param [in] recordRef A constant reference pointing to a record
        void AddRecord(const VaultRecordRef& recordRef);

        /// \brief Method for adding a recordы to VaultRequestResult
        /// \param [in] recordRef A constant reference indicating a vector with refs
        void AddRecordsRefs(const std::vector<VaultRecordRef> recordsRefs);

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
        VaultOperationResult GetRecord(const std::string& key, const T& keyValue, VaultRecordRef& vaultRecordRef) const
        {
            VaultOperationResult res;
            RecursiveReadWriteMtx.ReadLock();

            if (IsParentVaultValid)
            {
                ParentVault->RecursiveReadWriteMtx.ReadLock();

                res = Vault::GetRecord(key, keyValue, vaultRecordRef);
                vaultRecordRef.Vlt = ParentVault;
                
                ParentVault->RecursiveReadWriteMtx.ReadUnlock();
            }
            else 
            {
                res.Key = key;
                res.RequestedType = typeid(keyValue);
                res.IsOperationSuccess = false;
                res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
            }

            RecursiveReadWriteMtx.ReadUnlock();
            
            return res;
        }

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
        VaultOperationResult GetRecords(const std::string& key, const T& keyValue, std::vector<VaultRecordRef>& recordsRefs, const std::size_t& amountOfRecords = -1) const
        {
            VaultOperationResult res;
            RecursiveReadWriteMtx.ReadLock();

            if (IsParentVaultValid)
            {
                ParentVault->RecursiveReadWriteMtx.ReadLock();

                res = Vault::GetRecords(key, keyValue, recordsRefs);
                for (VaultRecordRef& ref: recordsRefs)
                    ref.Vlt = ParentVault;
                
                ParentVault->RecursiveReadWriteMtx.ReadUnlock();
            }
            else 
            {
                res.Key = key;
                res.RequestedType = typeid(keyValue);
                res.IsOperationSuccess = false;
                res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
            }

            RecursiveReadWriteMtx.ReadUnlock();
            
            return res;
        }

        /**
            \brief The method for getting the result of the request

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRequestResult A reference to VaultRequestResult
            \param [in] amountOfRecords The number of records requested
            
            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestRecords(const std::string& key, const T& keyValue, VaultRequestResult& vaultRequestResult) const
        {
            VaultOperationResult res;
            RecursiveReadWriteMtx.ReadLock();

            if (IsParentVaultValid)
            {
                ParentVault->RecursiveReadWriteMtx.ReadLock();

                // \todo Replace to reset to clear keys 
                vaultRequestResult.Clear();
                res = Vault::RequestRecords(key, keyValue, vaultRequestResult);
                vaultRequestResult.ParentVault = ParentVault;

                ParentVault->RecursiveReadWriteMtx.ReadUnlock();
            }
            else 
            {
                res.Key = key;
                res.RequestedType = typeid(keyValue);
                res.IsOperationSuccess = false;
                res.ResultCode = VaultOperationResultCode::ParentVaultNotValid;
            }

            RecursiveReadWriteMtx.ReadUnlock();
            
            return res;
        }

        /// \brief Resets the object to its initial state
        void Reset();

        /// \brief Clear VaultRequestResult
        /// Remove all references to records from the Vault Request Result, 
        /// but the records themselves in the original Vault will not be changed
        void Clear();

        // void RemoveRecordRef();

        // void RemoveRecordRefs();

        // void RemoveRecordRefs();

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
        void SortBy(const std::string& key, const F&& func, const bool& isReverse = false, const std::size_t& amountOfRecords = -1) const
        {
            RecursiveReadWriteMtx.ReadLock();

            // Thread safety because in Vault destructor blocking this RecursiveReadWriteMtx to write
            if (IsParentVaultValid)
            {
                ParentVault->RecursiveReadWriteMtx.ReadLock();
                Vault::SortBy(key, func, isReverse, amountOfRecords);
                ParentVault->RecursiveReadWriteMtx.ReadUnlock();
            }

            RecursiveReadWriteMtx.ReadUnlock();
        }

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
        ~VaultRequestResult();
    };
}