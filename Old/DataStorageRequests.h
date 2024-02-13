#pragma once

#include "DataStorageRecordSet.h"

/// Template interface for requests
/// \tparam <T> Any type of data except for c arrays
template <class T>
class DataStorageRequest
{
public:
    /// \brief Interface function for requests
    /// \param dataStorageMapStructure Map with all records in DataStorage
    /// \return A pair with iterators of the beginning and end of a block of data satisfying the request
    virtual std::pair<typename std::multimap<T, DataStorageRecord*>::iterator, typename std::multimap<T, DataStorageRecord*>::iterator> ProcessRequest(std::multimap<T, DataStorageRecord*>* dataStorageMapStructure) const = 0;
};

/// The template class of the request. Used to get all records greater or equal than a certain value
/// \tparam <T> Any type of data except for c arrays
template <class T>
class GreaterOrEqual : public DataStorageRequest<T>
{
public:
    /// The lower bound of the request
    T LowerBound;

    /**
        \brief Interface function for requests

        Returns all elements greater than or equal to LowerBound

        \param dataStorageMapStructure Map with all records in DataStorage

        \return A pair with iterators of the beginning and end of a block of data satisfying the request
    */
    virtual std::pair<typename std::multimap<T, DataStorageRecord*>::iterator, typename std::multimap<T, DataStorageRecord*>::iterator> ProcessRequest(std::multimap<T, DataStorageRecord*>* dataStorageMapStructure) const override 
    {
        return {dataStorageMapStructure->lower_bound(LowerBound), dataStorageMapStructure->end()};
    }

    /// A constructor that takes a template value to search for greater or equal elements
    /// \param [in] lowerBound The value to be compared with
    GreaterOrEqual(const T& lowerBound)
    {
        LowerBound = lowerBound;
    }
};

/// The template class of the request. Used to get all records greater than a certain value
/// \tparam <T> Any type of data except for c arrays
template <class T>
class Greater : public DataStorageRequest<T>
{
public:
    /// The lower bound of the request
    T LowerBound;

    /**
        \brief Interface function for requests

        Returns all elements greater than to LowerBound

        \param dataStorageMapStructure Map with all records in DataStorage

        \return A pair with iterators of the beginning and end of a block of data satisfying the request
    */
    virtual std::pair<typename std::multimap<T, DataStorageRecord*>::iterator, typename std::multimap<T, DataStorageRecord*>::iterator> ProcessRequest(std::multimap<T, DataStorageRecord*>* dataStorageMapStructure) const override 
    {
        return {dataStorageMapStructure->lower_bound(LowerBound), dataStorageMapStructure->end()};
    }

    /// A constructor that takes a template value to search for greater elements
    /// \param [in] lowerBound The value to be compared with
    Greater(const T& lowerBound)
    {
        LowerBound = lowerBound + 1;
    }
};

/// The template class of the request. Used to get all records less than a certain value or equal than a certain value
/// \tparam <T> Any type of data except for c arrays
template <class T>
class LessOrEqual : public DataStorageRequest<T>
{
public:
    
    /// The upper bound of the request
    T UpperBound;

    /**
        \brief Interface function for requests

        Returns all elements less than or equal to UpperBound

        \param dataStorageMapStructure Map with all records in DataStorage

        \return A pair with iterators of the beginning and end of a block of data satisfying the request
    */
    virtual std::pair<typename std::multimap<T, DataStorageRecord*>::iterator, typename std::multimap<T, DataStorageRecord*>::iterator> ProcessRequest(std::multimap<T, DataStorageRecord*>* dataStorageMapStructure) const override 
    {
        return {dataStorageMapStructure->begin(), dataStorageMapStructure->upper_bound(UpperBound)};
    }

    /// A constructor that takes a template value to search for less or equal elements
    /// \param [in] upperBound The value to be compared with
    LessOrEqual(const T& upperBound)
    {
        UpperBound = upperBound;
    }
};

/// The template class of the request. Used to get all records less than a certain value
/// \tparam <T> Any type of data except for c arrays
template <class T>
class Less : public DataStorageRequest<T>
{
public:
    /// The upper bound of the request
    T UpperBound;

    /**
        \brief Interface function for requests

        Returns all elements less than to LowerBound

        \param dataStorageMapStructure Map with all records in DataStorage

        \return A pair with iterators of the beginning and end of a block of data satisfying the request
    */
    virtual std::pair<typename std::multimap<T, DataStorageRecord*>::iterator, typename std::multimap<T, DataStorageRecord*>::iterator> ProcessRequest(std::multimap<T, DataStorageRecord*>* dataStorageMapStructure) const override 
    {
        return {dataStorageMapStructure->begin(), dataStorageMapStructure->lower_bound(UpperBound)};
    }

    /// A constructor that takes a template value to search for less elements
    /// \param [in] upperBound The value to be compared with
    Less(const T& upperBound)
    {
        UpperBound = upperBound;
    }
};

/// The template class of the request. Used to get all records less or equal than a certain value and greater or equal then different value
/// \tparam <T> Any type of data except for c arrays
template <class T>
class Between : public DataStorageRequest<T>
{
public:
    /// The bounds of the request
    T LowerBound, UpperBound;

    /**
        \brief Interface function for requests

        Returns all elements between LowerBound and UpperBound

        \param dataStorageMapStructure Map with all records in DataStorage

        \return A pair with iterators of the beginning and end of a block of data satisfying the request
    */
    virtual std::pair<typename std::multimap<T, DataStorageRecord*>::iterator, typename std::multimap<T, DataStorageRecord*>::iterator> ProcessRequest(std::multimap<T, DataStorageRecord*>* dataStorageMapStructure) const override 
    {
        if (LowerBound > UpperBound)
            return {dataStorageMapStructure->end(), dataStorageMapStructure->end()};
        else
            return {dataStorageMapStructure->lower_bound(LowerBound), dataStorageMapStructure->upper_bound(UpperBound)};
    }

    /// A constructor that accepts template parameters to search for all values between parameters
    /// If the lowerBound is greater than the upperBound, an empty set will be returned
    /// \param [in] upperBound The value to be compared with
    Between(const T& lowerBound, const T& upperBound)
    {
        UpperBound = upperBound;
        LowerBound = lowerBound;
    }
};

/**
    \defgroup DataStorageFunctions DataStorage functions
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

/**@} */