#pragma once

#include <list>
#include <vector>
#include <functional>

#include "VaultClasses.h"

#include "Map.h"
#include "VaultRecord.h"
#include "VaultRequest.h"
#include "VaultRecordRef.h"
#include "VaultParamInput.h"
#include "VaultOperationResult.h"

#include "Functions/CsvParser.h"
#include "Functions/PrintAsTable.h"

namespace mvlt
{
    /**
        \brief A class for storing data with the ability to quickly search for a variety of different keys of any type

        The class can be represented as a simple table with the ability to quickly search in O(1) for each field of the table.
        Each row of the table is called a record, and the VaultRecord class is used to store it. The column is called the key.
        Further, the keys will mean the columns of the table, and the records will mean the rows of the table.
        The SetKey function is used to add new keys. There is a template record inside the class and when adding a 
        key to the Vault, data will be added to this template record. When creating new records, they will be copied from this template record.
        Each record is unique, but the key values can be the same for many records.
        To work with records inside the Vault, the VaultRecordRef is used. You can use it to change the values of records inside the Vault.
    */
    class Vault
    {
    private:

        // Template of data stored inside Vault
        VaultRecord RecordTemplate;

        /*
            Hash map to store records
            A string with the name of the key is used as the key. All keys are the same as in Vault.
            The value stores a pointer to std::unordered_multimap<T, VaultRecord*>.
            The key type is same as the Vault key value type.
            The value is a pointer to VaultRecord.

            Such a complex structure is needed to quickly, in O(1), search for each key with any type.
        */
        mutable DataHashMap VaultHashMapStructure;

        /*
            Map to store records
            A string with the name of the key is used as the key. All keys are the same as in Vault.
            The value stores a pointer to std::multimap<T, VaultRecord*>.
            The key type is same as the Vault key value type.
            The value is a pointer to VaultRecord.

            Such a complex structure is necessary in order to quickly, in O(log n), 
            find a set of elements that meet a certain requirement, for example, more than a certain value or less than this value
        */
        mutable DataMap VaultMapStructure;

        // Unordered map with keys names and their types
        std::unordered_map<std::string, std::type_index> KeysTypes;

        // Unordered_map of functions that add a new element to the VaultStructureHashMap
        std::unordered_map<std::string, std::function<bool(VaultRecord*)>> VaultRecordAdders;

        // Unordered_map of functions which clean up the Vault structure, but do not delete records
        std::unordered_map<std::string, std::function<void()>> VaultRecordClearers;

        // Unordered_map of functions that erase record from the unordered_map's
        std::unordered_map<std::string, std::function<void(VaultRecord* newRecord)>> VaultRecordErasers;

        // Unordered_map of functions for getting sorted data.
        // The key is a string with the name of the key from the Vault.
        // The std::function is used as the value, in which the lambda function is written at the time of adding the key.
        // Lambda accepts a function that is called for each record inside. VaultRecordRef is passed to her. 
        // By default, iteration by records occurs in ascending order. 
        // isReverse parameter is used for the reverse order.
        std::unordered_map<std::string, std::function<void( std::function<bool(const VaultRecordRef&)> functionToSortedData, bool isReverse )>> VaultRecordSorters;

        // Unordered_map of functions that copy keys from this to VaultRecordSet
        std::unordered_map<std::string, std::function<void(VaultRecordSet& vaultRecordSet)>> VaultKeyCopiers;

        // List to store keys order
        std::list<std::string> KeysOrder;

        // Set with all unique keys
        std::set<std::string> UniqueKeys;

        // Vector with all invalid records in last readed file
        std::vector<std::pair<std::size_t, std::string>> InvalidFileRecords;

        // Recursive mutex for thread safety
        mutable RecursiveReadWriteMutex RecursiveReadWriteMtx;
    protected:

        /// \brief Enum for storing the object type during inheritance
        enum class VaultDerivedClasses
        {
            VaultBase,            ///< Vault class itself
            VaultRecordSetDerived ///< VaultRecordSet class
        };

        /// \brief A variable for storing the object type in the Vault inheritance tree
        VaultDerivedClasses VaultDerivedClass;

        /// \brief Unordered set with all VaultRecord pointers
        std::unordered_set<VaultRecord*> RecordsSet;

        /// \brief Unordered set with all VaultRecordSet pointers
        mutable std::unordered_set<VaultRecordSet*> RecordSetsSet;

        /**
            \brief Method for setting a new value in VaultRecord and Vault
            
            \tparam <T> - any type except c arrays

            \param [in] dataRecord pointer to VaultRecord inside Vault
            \param [in] key key to set data
            \param [in] data new data

            \return VaultOperationResult object with GetData result.
        */
        template <class T>
        VaultOperationResult SetDataToRecord(VaultRecord* dataRecord, const std::string& key, const T& data) noexcept;
        
        /**
            \brief Method for removing a record from a Vault

            This method allows you to remove a record

            \param [in] recordToErase the reference to the record that needs to be deleted
            \param [out] wasDeleted indicates whether the record has been deleted
            
            \return Returns an iterator to the RecordSet element immediately following the one being deleted. If the element to be deleted is not found, it returns RecordSet->end()
        */
        std::unordered_set<VaultRecord *>::iterator RemoveRecord(VaultRecord* recordToErase, bool* wasDeleted) noexcept;

        /**
            \brief The method for getting the result of the request in the std::set

            \tparam <T> Any type of data except for c arrays

            \param [in] requestType Type of request
            \param [in] key the name of the key to search for
            \param [in] beginKeyValue the begin value of the key to be found
            \param [in] endKeyValue the end value of the key to be found
            \param [in] isIncludeBeginKeyValue include beginKeyValue in the interval or not
            \param [in] isIncludeEndKeyValue include endKeyValue in the interval or not
            \param [in] recordsSet A reference to std::set<VaultRecord*> 
            \param [in] amountOfRecords The number of records requested. By default request all records
            \param [in] requestPredicat A function that accepts VaultRecordRef and decides whether to add an record to the request. 
            When the function returns true, the record is added, and when false is not added.

            \return VaultOperationResult object with RequestRecords result
        */
        template <class T>
        VaultOperationResult RequestRecordsSet(const VaultRequestType& requestType, const std::string& key, const T& beginKeyValue,
            const T& endKeyValue, std::unordered_set<VaultRecord*>& recordsSet, const bool& isIncludeBeginKeyValue, 
            const bool& isIncludeEndKeyValue, const std::size_t& amountOfRecords, const std::function<bool(const VaultRecordRef&)>& requestPredicat) const noexcept;

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
            \brief Template method to add new key with default value to Vault

            \tparam <T> Any type of data except for c arrays

            \param [in] key new key name
            \param [in] defaultKeyValue default key value
            \param [in] isUniqueKey is this is key with only unique values
            \param [in] isUniqueKeyWithoutLambda is try to add unique key without lambda. Vault must be empty
            \param [in] uniqueKeyFunction the function to add unique key in non-empty vault

            \return Returns false if such a key already exists, otherwise it returns true
        */
        template <class T>
        VaultOperationResult AddKey(const std::string& key, const T& defaultKeyValue, const bool& isUniqueKey, const bool& isUniqueKeyWithoutLambda,
            std::function<T(std::size_t, const VaultRecordRef&)> uniqueKeyFunction) noexcept;

        /**
            \brief A method for reading a csv file and loading data from it into memory

            \param [in] fileName the name of the file to read the data from
            \param [in] isPreprocessRecord a flag indicating the need to pre-process the record
            \param [in] recordHandler a function for preprocessing the recording. Accepts two vectors with strings. 
            The first vector contains the keys in the order in which they are specified in the file. 
            If the file does not contain keys, then this vector will be empty. The second vector is a vector of values.
            \param [in] separator the symbol to be used as a data separator in the file
            \param [in] isLoadKeys are there any keys in the file
            \param [in] userKeys vector with keys to read from file

            \return returns true if it was possible to read the file, otherwise it returns false
        */
        bool ReadFile(const std::string& fileName, const bool& isPreprocessRecord, std::function<void (const std::vector<std::string>&, std::vector<std::string>&)> recordHandler, 
            const char& separator, const bool& isLoadKeys, const std::vector<std::string> userKeys) noexcept;

    public:

        /// Making the VaultRecordRef class friendly so that it has access to the internal members of the Vault class
        friend VaultRecordRef;

        /// Making the VaultRecordSet class friendly so that it has access to the internal members of the Vault class
        friend VaultRecordSet;

        /// Making the VaultRequest class friendly so that it has access to the internal members of the Vault class
        template <VaultRequestType Type>
        friend class VaultRequest;

        /// Making VaultRecordSet operator comparison function friendly
        friend bool operator==(const VaultRecordSet& a, const VaultRecordSet& b);

        /// \brief Default constructor
        Vault() noexcept;

        /// \brief Deleted copy constructor
        Vault(const Vault& other) = delete;

        /// \brief Deleted assign operator
        Vault& operator= (const Vault& other) = delete;

        /**
            \brief Template method to add new key with default value to Vault

            \tparam <T> Any type of data except for c arrays

            \param [in] key new key name
            \param [in] defaultKeyValue default key value

            \return Returns false if such a key already exists, otherwise it returns true
        */
        template <class T>
        bool AddKey(const std::string& key, const T& defaultKeyValue) noexcept;

        /**
            \brief Template method to add new unique key to Vault

            \tparam <T> Any type of data except for c arrays

            Use it only on empty vault. If vault is not empty, then this method return error.

            \param [in] key new key name

            \return Returns VaultOperationResult with information about key adding
        */
        template <class T>
        VaultOperationResult AddUniqueKey(const std::string& key) noexcept;

        /**
            \brief Template method to add new unique key to Vault

            \tparam <T> Any type of data except for c arrays

            \param [in] key new key name
            \param [in] uniqueKeyFunction A function to determine the value of the key that will be set
             for an entry that has already been in vault. The function takes std::size_t as the index of the record. 
             The same index value between runs is not guaranteed. The function should return an object of type T, which will be set to a record.

            The basic syntax of the method is as follows:
            \code{.cpp}
                vlt.AddUniqueKey<int>("A", {[](std::size_t counter, const VaultRecordRef& vrf) -> int { return static_cast<int>(counter); }});
            \endcode
            Note that the lambda function is enclosed in {} and the return value is specified via ->

            \return Returns VaultOperationResult with information about key adding
        */
        template <class T>
        VaultOperationResult AddUniqueKey(const std::string& key, std::function<T(std::size_t, const VaultRecordRef&)> uniqueKeyFunction) noexcept;

        /**
            \brief Template method to update default key value

            \tparam <T> Any type of data except for c arrays

            \param [in] key new key name
            \param [in] defaultKeyValue new default key value

            The type of the defaultKeyValue must match the type of the old value

            \return Returns VaultOperationResult with the result of the operation
        */
        template <class T>
        VaultOperationResult UpdateKey(const std::string& key, const T& defaultKeyValue) noexcept;

        /**
            \brief The method for getting a default key value

            \param [in] key the name of the key to search for

            \return returns true if the key was found otherwise returns false
        */
        bool IsKeyExist(const std::string& key) const noexcept;
        
        /**
            \brief The method for getting a default key value

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] defaultKeyValue the value of the key

            \return VaultOperationResult with operation result
        */
        template <class T>
        VaultOperationResult GetKeyValue(const std::string& key, T& defaultKeyValue) const noexcept;

        /**
            \brief The method for getting a key type

            \param [in] key the name of the key to search for
            \param [in] keyType the ref to std::type_index

            \return returns true if the key was found otherwise returns false
        */
        bool GetKeyType(const std::string& key, std::type_index& keyType) const noexcept;

        /**
            \brief The method for getting all the keys

            \return vector with keys
        */
        std::vector<std::string> GetKeys() const noexcept;

        /**
            \brief The method for deleting the key

            \param [in] key the key to remove

            \return Returns true if the key existed, otherwise returns false
        */
        bool RemoveKey(const std::string& key) noexcept;

        /**
            \brief Method to create new VaultRecord.

            The method accepts a vector of pairs, the first element of the pair is a string with a key, 
            and the second element of the pair are key values of any type.
            The order of the pairs is not important, assignment takes place by key. The number of pairs can be any, 
            for all keys for which no value has been specified, the default value will remain.
            If there are unique keys in the vault, then when you try to add a record with a 
            unique key value that is already in the vault, the new record will not be added.

            The Vault in the example has 2 keys. One is the id of the int type, and the second is the name of the std::string type
            Usage example:

            \code{.cpp}
                // A record with id 0 and name mrognor will be created
                vlt.CreateRecord({ {"id", 0}, {"name", std::string("mrognor")} });
            \endcode

            or

            \code{.cpp}
                // A record with id 0 and name mrognor will be created
                vlt.CreateRecord({ {"name", std::string("mrognor")}, {"id", 0} });
            \endcode

            or

            \code{.cpp}
                // A record with name mrognor will be created. The Id will be set to the default value
                vlt.CreateRecord({ {"name", std::string("mrognor")} });
            \endcode

            or

            \code{.cpp}
                // A record with id 0 and name mrognor will be created
                std::vector<std::pair<std::string, VaultParamInput>> params = { {"id", 0}, {"name", std::string("mrognor")} };
                VaultRecordRef vltrr = vlt.CreateRecord(params);
            \endcode

            what is equivalent to such a code without passing values to a function

            \code{.cpp}
                VaultRecordRef vltrr = vlt.CreateRecord({});

                vltrr.SetData("id", 0);
                vltrr.SetData<std::string>("name", "mrognor");
            \endcode

            \param [in] params a vector of pairs with data to be put in the Vault

            If there was an error in one of the parameters, the function will stop at this parameter and return an error for this parameter.
            
            \return VaultOperationResult object with CreateRecord result
        */
        VaultOperationResult CreateRecord(const std::vector<std::pair<std::string, VaultParamInput>>& params) noexcept;

        /**
            \overload
            \brief Method to create new VaultRecord.

            If there are unique keys in the vault, then when you try to add a record with a unique key value that is already in the vault, 
            the new record will not be added, and the record created earlier will be placed in vaultRecordRef.

            \param [in] vaultRecordRef The reference to the VaultRecordRef to which the new record will be assigned
            \param [in] params a vector of pairs with data to be put in the Vault

            \return VaultOperationResult object with CreateRecord result
        */
        VaultOperationResult CreateRecord(VaultRecordRef& vaultRecordRef, const std::vector<std::pair<std::string, VaultParamInput>>& params) noexcept;

        /**
            \brief The method for getting a reference to the data inside Vault

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] vaultRecordRef A reference to VaultRecordRef, where information about the requested record will be recorded.
            If the key is not found, or the saved type does not match the type T, the parameter will not change.
            If the key was found and the types matched, but the value was not found, 
            a nullptr record will be saved to the ref and the vaultRecordRef.isValid() method returns false.

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
            const std::size_t& amountOfRecords = -1, const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const noexcept;

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
            const std::size_t& amountOfRecords = -1, const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const noexcept;

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
            const std::size_t& amountOfRecords = -1, const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const noexcept;

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
            const std::size_t& amountOfRecords = -1, const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const noexcept;

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
            const std::size_t& amountOfRecords = -1, const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const noexcept;

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
            const std::function<bool(const VaultRecordRef&)>& requestPredicat = DefaultRequestPredicat) const noexcept;

        /**
            \brief A method for complex requests

            \param [in] request The request
            \param [in] vaultRecordSet Set, to save the query result
        */
        template <VaultRequestType Type>
        VaultOperationResult Request(const VaultRequest<Type>&& request, VaultRecordSet& vaultRecordSet) const;

        /**
            \brief A method for deleting all data and keys
        
            \warning This method is not thread-safe! Call it only if you are sure that other threads are not working with VaultRecordSet or VaultRecordRef received from Vault
        */
        void DropVault() noexcept;

        /// \brief A method for deleting all data, but keeping all keys
        void DropData() noexcept;

        /**
            \brief Method for deleting a record from a Vault
            \param [in] recordRefToErase the reference to the record that needs to be deleted
            \return Returns true if the record existed and was successfully deleted, otherwise it returns false
        */
        bool EraseRecord(const VaultRecordRef& recordRefToErase) noexcept;

        /**
            \brief The method for erase record using key and value

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            
            \return VaultOperationResult object with EraseRecord result
        */
        template <class T>
        VaultOperationResult EraseRecord(const std::string& key, const T& keyValue) noexcept;

        /**
            \brief The method for erase records using key and value

            \tparam <T> Any type of data except for c arrays

            \param [in] key the name of the key to search for
            \param [in] keyValue the value of the key to be found
            \param [in] amountOfRecords The number of records to delete. By default set to minus one or all records.
            
            If the amountOfRecords is greater than the number of records stored inside the Vault, then all records with this key and value will be deleted.

            \return VaultOperationResult object with EraseRecords result
        */
        template <class T>
        VaultOperationResult EraseRecords(const std::string& key, const T& keyValue, const std::size_t& amountOfRecords = -1) noexcept;

        /**
            \brief Method for getting the number of records
            \return number of records
        */
        std::size_t Size() const noexcept;

        /**
            \brief Method for getting sorted records

            \param [in] key The key by which the data should be sorted
            \param [in] isReverse Sort in descending order or descending order. By default, ascending
            \param [in] amountOfRecords The number of records. By default, everything is

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

            If func returns false, then SortBy will return.
            The function iterate over all records sorted by the key parameter, in the order specified by the isReverse parameter. 
            For each record, the function passed in the func parameter is called.
            This function does not sort the data when it is called, the sorted data is already stored inside the Vault.
            If the key is missing in the vault, the function will be called 0 times
        */
        template<class F>
        void SortBy(const std::string& key, const F&& func, const bool& isReverse = false, const std::size_t& amountOfRecords = -1) const noexcept;

        /**
            \brief A method for displaying the contents of a Vault on the screen
            \param [in] amountOfRecords The number of records to be printed. The default value is -1, which means that all entries will be output
        */
        void PrintVault(const std::size_t& amountOfRecords = -1) const noexcept;
        
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
            \brief A method for saving data to a table file. The file format is csv

            \param [in] fileName The file name to save the data, the extension must be specified manually
            \param [in] separator The symbol that will be used to separate the record elements in the file
            \param [in] isSaveKey A variable that determines whether to save keys to a file

            \return It will return true if it was possible to open the file and write the data, otherwise it will return false
        */
        bool SaveToFile(const std::string& fileName, const std::vector<std::string> keys = {}, const std::string& separator = ",", const bool& isSaveKey = true) const noexcept;

        /**
            \brief A method for reading a csv file and loading data from it into memory

            \param [in] fileName the name of the file to read the data from
            \param [in] separator the symbol to be used as a data separator in the file
            \param [in] isLoadKeys are there any keys in the file
            \param [in] keys vector with keys to read from file. Useless if isLoadKeys == true

            \return returns true if it was possible to read the file, otherwise it returns false
        */
        bool ReadFile(const std::string& fileName, const char& separator = ',', const bool& isLoadKeys = true, const std::vector<std::string> keys = {}) noexcept;

        /**
            \brief A method for reading a csv file and loading data from it into memory

            \param [in] fileName the name of the file to read the data from
            \param [in] separator The symbol to be used as a data separator in the file
            \param [in] isLoadKeys Are there any keys in the file
            \param [in] recordHandler a function for preprocessing the recording. Accepts two vectors with strings. 
            The first vector contains the keys in the order in which they are specified in the file. 
            If the file does not contain keys, then this vector will be empty. The second vector is a vector of values.

            \return returns true if it was possible to read the file, otherwise it returns false
        */
        bool ReadFile(const std::string& fileName, const char& separator, const bool& isLoadKeys, std::function<void (const std::vector<std::string>&, std::vector<std::string>&)> recordHandler) noexcept;

        /**
            \brief A method for getting errors in the last read file

            In the returned vector, the first element is the error string, and the second is the key whose value could not be read.

            \return vector with errors
        */
        std::vector<std::pair<std::size_t, std::string>> GetErrorsInLastReadedFile() const noexcept;

        /// \brief Default destructor
        ~Vault() noexcept;

        /// Making Intersection function friendly
        friend void Intersection(const VaultRecordSet& a, const VaultRecordSet& b, VaultRecordSet& res) noexcept;
    };
}