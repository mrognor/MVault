#pragma once

#include <unordered_set>

#include "DataStorageClasses.h"
#include "DataStorageRecord.h"

/// A class for storing set of DataStorageRecordRef's
class DataStorageRecordSet
{
private:
    // Unordered set with all DataStorageRecordRef's
    std::unordered_set<DataStorageRecordRef> RecordsSet;
public:

    /// Redefine iterator from C std::unordered_set
    typedef typename std::unordered_set<DataStorageRecordRef>::iterator iterator;
    /// Redefine const_iterator from C std::unordered_set
    typedef typename std::unordered_set<DataStorageRecordRef>::const_iterator const_iterator;

    /// Returns a read-only (constant) iterator that points to the first element in the DataStorageRecordSet
    /// \return begin iterator
    iterator begin() noexcept;
    
    /// Returns a read-only (constant) iterator that points to the first element in the DataStorageRecordSet
    /// \return begin iterator
    const_iterator begin() const noexcept;

    /// Returns a read-only (constant) iterator that points to the first element in the DataStorageRecordSet
    /// \return begin iterator
    const_iterator cbegin() const noexcept;

    /// Returns a read-only (constant) iterator that points one past the last element in the DataStorageRecordSet
    /// \return end iterator
    iterator end() noexcept;

    /// Returns a read-only (constant) iterator that points one past the last element in the DataStorageRecordSet
    /// \return end iterator
    const_iterator end() const noexcept;

    /// Returns a read-only (constant) iterator that points one past the last element in the DataStorageRecordSet
    /// \return end iterator
    const_iterator cend() const noexcept;

    /// \brief A method for adding a new DataStorageRecordRef inside a DataStorageRecordRefSet
    /// \param [in] newRecordRefPtr the object to be added to the set
    void AddNewRecord(const DataStorageRecordRef& newRecordRefPtr);

    /**
        \brief A method for adding a new DataStorageRecordRef inside a DataStorageRecordRefSet

        \param [in] newRecordPtr a pointer to an record in DataStorage
        \param [in] dataStorageStructureHashMap pointer to the DataStorageHashMap structure
        \param [in] dataStorageStructureMap pointer to the DataStorageMap structure
    */
    void AddNewRecord(DataStorageRecord* newRecordPtr, DataStorageStructureHashMap* dataStorageStructureHashMap, DataStorageStructureMap* dataStorageStructureMap);

    /// \brief Returns the size of the DataStorageRecordSet
    /// \return size of the DataStorageRecordSet
    std::size_t Size() const;

    /// Erases all elements in an DataStorageRecordSet
    void Clear();

    /// \brief A method for checking the presence of an element in a set
    /// \param [in] dataToCheck the object to check for
    /// \return returns true if there is such an element, otherwise it returns false
    bool Contain(const DataStorageRecordRef& dataToCheck) const;

    /// A method for combining sets
    /// \param [in] other the set that will be added to this
    void Join(const DataStorageRecordSet& other);
};

/*! 
    \defgroup functions DataStorage functions
    \brief All functions for working with DataStorage

    @{      
*/

/**
    \brief A function for union two DataStorageRecordSet

    \param [in] A The first set to union
    \param [in] B The second set to union

    \return A new set containing all the elements from A and B. All the elements in it are unique
*/ 
DataStorageRecordSet Union(const DataStorageRecordSet& A, const DataStorageRecordSet& B);

/**
    \brief A function for excluding elements of one set if that is inside another set

    \param [in] A The set from which should be excluded
    \param [in] B A set whose elements must be excluded

    \return A new set that contains all the elements from A that are not in B
*/ 
DataStorageRecordSet Except(const DataStorageRecordSet& A, const DataStorageRecordSet& B);

/**
    \brief A function for the intersection of elements from two sets

    \param [in] A The first set to intersection
    \param [in] B The second set to intersection

    \return A new set that contains only elements that exist in both A and B
*/ 
DataStorageRecordSet Intersection(const DataStorageRecordSet& A, const DataStorageRecordSet& B);

/**
    \brief A function for the alternative intersection of elements from two sets

    This function works as a reverse intersection, i.e. 
    it will return only elements from A that are not in B and elements from B that are not in A

    \param [in] A The first set to alter intersection
    \param [in] B The second set to alter intersection

    \return A new set in which there are all elements from A and B that are not in A and B at the same time
*/ 
DataStorageRecordSet AlterIntersection(const DataStorageRecordSet& A, const DataStorageRecordSet& B);

/*! @} */