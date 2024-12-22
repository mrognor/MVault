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
            \param [in] requestPredicat A function that accepts VaultRecordRef and decides whether to add an record to the request. 
            When the function returns true, the record is added, and when false is not added.

            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestRecords(const VaultRequestType& requestType, const std::string& key, const T& beginKeyValue,
            const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue, 
            const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept;

        /**
            \brief Method for removing a record from a Vault

            This method allows you to remove a record from VaultRecord set without deletion in Vault

            \param [in] recordToErase the reference to the record that needs to be deleted
            \param [out] wasDeleted indicates whether the record has been deleted
            
            \return Returns an iterator to the RecordSet element immediately following the one being deleted. If the element to be deleted is not found, it returns RecordSet->end()
        */
        std::unordered_set<VaultRecord *>::iterator RemoveRecord(VaultRecord* recordToErase, bool* wasDeleted) noexcept;

        /**
            \brief An internal method for copying objects

            \param [in] other object to copy
        */
        void CopySet(const VaultRecordSet& other) noexcept;

        /**
            \brief An internal method for moving objects

            \param [in] other object to move
        */
        void MoveSet(VaultRecordSet& other) noexcept;

    public:

        /// Make Vault class friend
        friend Vault;

        /// Make VaultRecord class friend
        friend VaultRecord;

        /// \brief Default constructor
        VaultRecordSet() noexcept;

        /**
            \brief Copy constructor

            \param [in] other object to copy
        */
        VaultRecordSet(const VaultRecordSet& other) noexcept;

        /**
            \brief Operator assignment

            \param [in] other object to copy

            \return reference to copied object
        */
        VaultRecordSet& operator=(const VaultRecordSet& other) noexcept;

        /**
            \brief Move constructor

            \param [in] other object to move
        */
        VaultRecordSet(VaultRecordSet&& other) noexcept;

        /**
            \brief Move assignment perator

            \param [in] other object to move

            \return reference to moveed object
        */
        VaultRecordSet& operator=(VaultRecordSet&& other) noexcept;

        /// Making operator comparison function friendly
        friend bool operator==(const VaultRecordSet& a, const VaultRecordSet& b);

        /**
            \brief Method for checking the validity of the parent Vault
            
            \return true if it is valid, otherwise false
        */
        bool GetIsParentVaultValid() const noexcept;

        /**
            \brief Method for getting parent vault unique id
            
            \return id or null in case invalid parent
        */
        std::string GetParentVaultUniqueId() const noexcept;
    
        /**
            \brief The method for getting a default key value

            \param [in] key the name of the key to search for

            \return returns false if the parent vault invalid or key was not found otherwise returns true
        */
        bool IsKeyExist(const std::string& key) const noexcept;

        /**
            \brief The method for getting a default key value

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] defaultKeyValue the value of the key
        */
        template <class T>
        VaultOperationResult GetKeyValue(const std::string& key, T& defaultKeyValue) const noexcept;

        /**
            \brief The method for getting a key type

            \param [in] key the name of the key to search for
            \param [in] keyType the ref to std::type_index

            \return returns false if the parent vault invalid or the key was not found otherwise returns true
        */
        bool GetKeyType(const std::string& key, std::type_index& keyType) const noexcept;

        /**
            \brief Method for adding a record to VaultRecordSet
            
            \param [in] recordRef A constant reference pointing to a record
        */
        VaultOperationResult AddRecord(const VaultRecordRef& recordRef) noexcept;

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
        VaultOperationResult GetRecord(const std::string& key, const T& keyValue, VaultRecordRef& vaultRecordRef) const noexcept;

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
        VaultOperationResult GetRecords(const std::string& key, const T& keyValue, std::vector<VaultRecordRef>& recordsRefs, const std::size_t& amountOfRecords = -1) const noexcept;

        /**
            \brief A method for getting all records that have a value equal to keyValue stored by the key key

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            \param [in] requestPredicat A function that accepts VaultRecordRef and decides whether to add an record to the request. 
            When the function returns true, the record is added, and when false is not added.

            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
            const std::size_t& amountOfRecords = -1, const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const;

        /**
            \brief A method for getting all records that have a value greater than keyValue stored by the key key

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            \param [in] requestPredicat A function that accepts VaultRecordRef and decides whether to add an record to the request. 
            When the function returns true, the record is added, and when false is not added.

            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestGreater(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
            const std::size_t& amountOfRecords = -1, const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const;

        /**
            \brief A method for getting all records that have a value greater than or equal to keyValue stored by the key key

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            \param [in] requestPredicat A function that accepts VaultRecordRef and decides whether to add an record to the request. 
            When the function returns true, the record is added, and when false is not added.

            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestGreaterOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
            const std::size_t& amountOfRecords = -1, const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const;

        /**
            \brief A method for getting all records that have a value less than keyValue stored by the key key

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            \param [in] requestPredicat A function that accepts VaultRecordRef and decides whether to add an record to the request. 
            When the function returns true, the record is added, and when false is not added.

            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestLess(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
            const std::size_t& amountOfRecords = -1, const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const;

        /**
            \brief A method for getting all records that have a value less than or equal to keyValue stored by the key key

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordSet
            \param [in] amountOfRecords The number of records requested. By default request all records
            \param [in] requestPredicat A function that accepts VaultRecordRef and decides whether to add an record to the request. 
            When the function returns true, the record is added, and when false is not added.

            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestLessOrEqual(const std::string& key, const T& keyValue, VaultRecordSet& vaultRecordSet,
            const std::size_t& amountOfRecords = -1, const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const;

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
            \param [in] requestPredicat A function that accepts VaultRecordRef and decides whether to add an record to the request. 
            When the function returns true, the record is added, and when false is not added.

            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestInterval(const std::string& key, const T& beginKeyValue,
            const T& endKeyValue, VaultRecordSet& vaultRecordSet, const bool& isIncludeBeginKeyValue = true, 
            const bool& isIncludeEndKeyValue = true, const std::size_t& amountOfRecords = -1,
            const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const;
        
        /**
            \brief A method for complex requests

            \param [in] request The request
            \param [in] vaultRecordSet Set, to save the query result
        */
        template <VaultRequestType Type>
        VaultOperationResult Request(const VaultRequest<Type>&& request, VaultRecordSet& vaultRecordSet) const;

        /**
            \brief A method for checking for an entry in set

            \param [in] ref ref to the record to check for availability in the set

            \return returns true if the record is in set, otherwise false
        */
        bool CheckRecord(const VaultRecordRef& ref) const noexcept;

        /// \brief Resets the object to its initial state
        void Reset() noexcept;

        /**
            \brief Clear VaultRecordSet
            
            Remove all references to records from the Vault Request Result, 
            but the records themselves in the original Vault will not be changed
        */
        void Clear() noexcept;

        /**
            \brief Method for remove a record from a Vault

            The record itself will not be changed in any way

            \param [in] recordRefToErase the reference to the record that needs to be removed

            \return Returns true if the record existed and was successfully removed, otherwise it returns false
        */
        bool RemoveRecord(const VaultRecordRef& recordRefToErase) noexcept;

        /**
            \brief Method for remove a record from a Vault

            \tparam <T> Any type of data except for c arrays

            The record itself will not be changed in any way

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            
            \return VaultOperationResult object with RemoveRecord result
        */
        template <class T>
        VaultOperationResult RemoveRecord(const std::string& key, const T& keyValue) noexcept;

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
        VaultOperationResult RemoveRecords(const std::string& key, const T& keyValue, const std::size_t& amountOfRecords = -1) noexcept;

        /**
            \brief Method for getting the number of records
            
            If the parent Vault is not valid, it will return 0
            
            \return number of records
        */
        std::size_t Size() const noexcept;

        /**
            \brief The method for getting all the keys
            
            If the parent Vault is not valid, it will return an empty vector
            
            \return vector with keys
        */
        std::vector<std::string> GetKeys() const noexcept;

        /**
            \brief Method for getting sorted records

            \param [in] key The key by which the data should be sorted
            \param [in] isReverse Sort in descending order or descending order. By default, ascending
            \param [in] amountOfRecords The number of records. By default, everything is

            If the parent Vault is not valid, it will return an empty vector
            If the key is missing in the vault, the result vector will be empty
            \return A vector with links to records. The order of entries in the vector is determined by the amountOfRecords parameter
        */
        std::vector<VaultRecordRef> GetSortedRecords(const std::string& key, const bool& isReverse = false, const std::size_t& amountOfRecords = -1) const noexcept;

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
        void SortBy(const std::string& key, const F&& func, const bool& isReverse = false, const std::size_t& amountOfRecords = -1) const noexcept;

        /**
            \brief Method for saving the contents of the VaultRecordSet in json

            \param [in] isFormat is line formatting required, i.e. adding line breaks and tabs
            \param [in] tabSize the number of spaces when tabbing
            \param [in] isUseRecordTemplate will recordTemplate be used for the title of the record or will an internal unique identifier be used
            \param [in] recordTemplate a template for the record name to which the record number will be added

            \return json string
        */
        std::string ToJson(const bool& isFormat = false, const std::size_t& tabSize = 2, const bool& isUseRecordTemplate = true,
            const std::string& recordTemplate = "Record") const noexcept;

        /**
            \brief A method for displaying the contents of a Vault on the screen
            
            \param [in] amountOfRecords The number of records to be printed. The default value is -1, which means that all entries will be output
        */
        void PrintSet(const std::size_t& amountOfRecords = -1) const noexcept;
        
        /**
            \brief A method for displaying the contents of a Vault as a table on the screen
            
            \param [in] isPrintId will the unique IDs be printed in the table
            \param [in] amountOfRecords The number of records to be printed. The default value is -1, which means that all entries will be output
            \param [in] primaryKey The key by which the data will be sorted
            \param [in] isReverse The sorting direction. If true, the data will be output from the larger to the smaller
            \param [in] keys list of keys to be printed. By default, the list is empty, which means that all keys will be output
        */
        void PrintAsTable(bool isPrintId = false, const std::size_t& amountOfRecords = -1, std::string primaryKey = "", const bool& isReverse = false,
            const std::list<std::string> keys = {}) const noexcept;

        /**
            \brief A method for adding all records from a to this
            
            \param [in] a the set from which the recordings will be added

            \return join result
        */
        VaultOperationResult Join(const VaultRecordSet& a) noexcept;

        /**
            \brief A method for deleting all records from this that are also in a
            
            \param [in] a the record set that contains the records to remove
        */
        void Exclude(const VaultRecordSet& a) noexcept;
        
        /**
            \brief A method to delete all records from this that are not in a
            
            \param [in] a a set that contains records that do not need to be removed
        */
        void Intersect(const VaultRecordSet& a) noexcept;

        /**
            \brief A method for saving data to a table file. The file format is csv

            \param [in] fileName The file name to save the data, the extension must be specified manually
            \param [in] keys a vector with the keys to save. The keys will be stored in the same order as in the vector
            \param [in] separator The symbol that will be used to separate the record elements in the file
            \param [in] isSaveKey A variable that determines whether to save keys to a file

            The validity of the keys is provided by the user. In case of a non-existent key, an empty column will be written to the file.
            If parent Vault not valid return false
            \return It will return true if it was possible to open the file and write the data, otherwise it will return false
        */
        bool SaveToFile(const std::string& fileName, const std::vector<std::string> keys = {}, const std::string& separator = ",", const bool& isSaveKey = true) const noexcept;

        /// \brief Destructor
        ~VaultRecordSet() noexcept;

        /// Making Union function friendly
        friend void Union(const VaultRecordSet& a, const VaultRecordSet& b, VaultRecordSet& res) noexcept;

        /// Making Intersection function friendly
        friend void Intersection(const VaultRecordSet& a, const VaultRecordSet& b, VaultRecordSet& res) noexcept;
    };

    /**
        \brief Friend comparison operator
            
        \param [in] a first set to compare
        \param [in] b second set to compare
        
        \return true if a equal b, false otherwise
    */
    bool operator==(const VaultRecordSet& a, const VaultRecordSet& b);
}