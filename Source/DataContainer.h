#pragma once

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

#include "DataSaver.h"

/**
    \brief Template prototype of a container for storing different types of data.

    \tparam <C> The container class for which this class will be the wrapper. 
    Container have to support find(), begin(), end(), emplace(), erase().  
    Types that are used in inheritors:
    \tparam <std::set> A container built on an avl tree.
    \tparam <std::map> A container built on an avl tree. Stores a pair of key and value.
    \tparam <std::unordered_set> A container built on a hash table.
    \tparam <std::unordered_map> A container built on a hash table. Stores a pair of key and value.

    The container should work according to the std::map type, i.e. store a key and value pair in itself.
    Using class specialization, you can define an internal container. 
    If a pointer is written to one of the elements, then you can set a custom data deletion function.
*/
template <class C>
class DataContainer
{
protected:
    /// Container to store all data inside C container
    C Container;

public:
    /// Redefine iterator from C container
    typedef typename C::iterator iterator;

    /// Redefine const_iterator from C container
    typedef typename C::const_iterator const_iterator;

    /// Begin provides access to the Data iterator
    inline iterator begin() noexcept { return Container.begin(); }
    /// Cbegin provides access to the Data const_iterator
    inline const_iterator cbegin() const noexcept { return Container.cbegin(); }

    /// End provides access to the Data iterator
    inline iterator end() noexcept { return Container.end(); }
    /// Cend provides access to the Data const_iterator
    inline const_iterator cend() const noexcept { return Container.cend(); }

    /**
        \brief Template method for adding a new data to the container.

        \tparam <T> Any type of data except for c arrays

        Wrapper over emplace std container

        \param [in] key key for storing the data
        \param [in] data data to store in container
    */
    template <class T>
    void AddData(const std::string& key, const T& data)
    {
        Container.emplace(key, DataSaver(data));
    }

    /**
        \brief Template method for adding a new data to the container and a function to delete this data

        \tparam <T> Any type of data except for c arrays
        \tparam <F> Function pointer or lambda function

        Wrapper over emplace std container
        
        \param [in] key key for storing the data
        \param [in] data data to store in container
        \param [in] deleteFunc function to delete data
    */
    template <class T, class F>
    void AddData(const std::string& key, const T& data, F&& deleteFunc)
    {
        Container.emplace(key, DataSaver(data, deleteFunc));
    }

    /**
        \brief Method for adding a new data to the container

        \param [in] key the key whose value needs to be changed
        \param [in] dataSaver dataSaver with any type, the value from which will be copied to the container 
    */
    void AddDataFromDataSaver(const std::string& key, const DataSaver& dataSaver)
    {
        Container.emplace(key, dataSaver);
    }

    /**
        \brief Method for changing the value of a data inside a container using a key

        \tparam <T> Any type of data except for c arrays

        If there was no data with such a key, it will be created

        \param [in] key the key whose value needs to be changed
        \param [in] data new key value
    */
    template <class T>
    void SetData(const std::string& key, const T& data)
    {
        SetData(key, data, nullptr);
    }

    /**
        \brief Method for changing the value of a data inside a container using a key

        \tparam <T> Any type of data except for c arrays
        \tparam <F> Function pointer or lambda function

        If there was no data with such a key, it will be created

        \param [in] key the key whose value needs to be changed
        \param [in] data new key value
        \param [in] deleteFunc function to delete data
    */
    template <class T, class F>
    void SetData(const std::string& key, const T& data, F&& deleteFunc)
    {
        // Find data iterator
        auto f = Container.find(key);

        // Checking whether there was such a key in the container
        if (f == Container.end())
            AddData(key, data, deleteFunc); // Adding data to the container if there was no key
        else
            f->second.SetData(data, deleteFunc); // Setting new data for the key if it was already in the container
    }

    /**
        \brief Method for changing the value of a data inside a container using a key

        If there was no data with such a key, it will be created

        \param [in] key the key whose value needs to be changed
        \param [in] dataSaver dataSaver with any type, the value from which will be copied to the container 
    */
    void SetDataFromDataSaver(const std::string& key, const DataSaver& dataSaver)
    {
        // Find data iterator
        auto f = Container.find(key);

        // Checking whether there was such a key in the container
        if (f == Container.end())
            AddDataFromDataSaver(key, dataSaver); // Adding data to the container if there was no key
        else
            f->second = dataSaver; // Setting new data for the key if it was already in the container
    }

    /**
        \brief Method for getting data from a container using a key

        \tparam <T> Any type of data except for c arrays

        \param [in] key key for getting data
        \param [out] data a reference to data of type T to write data from the container there.
        If the data was not found, then nothing will be written to the data

        \return Returns false if the key was not found, and otherwise returns true.
    */ 
    template <class T>
    bool GetData(const std::string& key, T& data) const
    {
        auto f = Container.find(key);
        if (f == Container.end())
            return false;

        f->second.GetData(data);
        return true;
    }

    /**
        \brief Method for getting dataSaver from a container using a key

        \param [in] key key for getting data
        \param [out] dataSaver a reference to dataSaver to write data from the container there.
        If the data was not found, then nothing will be written to the dataSaver

        \return Returns false if the key was not found, and otherwise returns true.
    */ 
    bool GetDataSaver(const std::string& key, DataSaver& dataSaver) const
    {
        auto f = Container.find(key);
        if (f == Container.end())
            return false;

        dataSaver = f->second;
        return true;
    }
    
    /// \brief A method for checking whether data with such a key is in the container
    /// \param [in] key key to find in container
    /// \return Returns false if the key was not found, and otherwise returns true.
    bool IsData(const std::string& key) const
    {
        return Container.find(key) != Container.end();
    }

    /**
        \brief Function for erasing data from a container

        If a custom deletion function has been set for the data, it will be called

        \param [in] key key to find in container
    */
    void EraseData(const std::string& key)
    {
        auto f = Container.find(key);
        if (f != Container.end())
        {
            f->second.ResetData();
            Container.erase(f);
        }
    }

    /// \brief Method for clear all data inside container
    /// Note that if pointers were stored in the container, they must be cleaned manually
    void Clear()
    {
        Container.clear();
    }

    /// \brief Method for getting the container size
    /// \return Container size
    std::size_t Size() const
    {
        return Container.size();
    }
};

/// Specialization of class DataContainer, used as a type std::unordered_map<std::string, DataSaver>
class DataHashMap : public DataContainer<std::unordered_map<std::string, DataSaver>> {};

/// Specialization of class DataContainer, used as a type std::unordered_multimap<std::string, DataSaver>
class DataMultiHashMap : public DataContainer<std::unordered_multimap<std::string, DataSaver>>
{
public:
    /**
        \brief A function for searching for multiple elements with the same keys

        Wrapper over std::unordered_multimap<std::string, DataSaver>::equal_range

        \param [in] key key to find in container

        \return pair of iterators. The first iterator points to the first element with the same key, and the second to the last element
    */
    std::pair<DataContainer::const_iterator, DataContainer::const_iterator> GetAllData(const std::string& key) const
    {
        return Container.equal_range(key);
    }
};

/// Specialization of class DataContainer, used as a type std::map<std::string, DataSaver>
class DataMap : public DataContainer<std::map<std::string, DataSaver>> {};

/// Specialization of class DataContainer, used as a type std::multimap<std::string, DataSaver>
class DataMultiMap : public DataContainer<std::multimap<std::string, DataSaver>>
{
public:
    /**
        \brief A function for searching for multiple elements with the same keys

        Wrapper over std::unordered_multimap<std::string, DataSaver>::equal_range

        \param [in] key key to find in container

        \return pair of iterators. The first iterator points to the first element with the same key, and the second to the last element
    */
    std::pair<DataContainer::const_iterator, DataContainer::const_iterator> GetAllData(const std::string& key) const
    {
        return Container.equal_range(key);
    }
};

/**
    A simple typedef for HashMap. It is necessary for a more understandable separation of types.
    Represents the internal structure of the DataStorage.
    A string with the name of the key is used as the key. All keys are the same as in DataStorage.
    The value stores a pointer to std::unordered_multimap<T, DataStorageRecord*>.
    The key type is same as the DataStorage key value type.
    The value is a pointer to DataStorageRecord.

    Such a complex structure is needed to quickly, in O(1), search for each key with any type.
*/
typedef DataHashMap DataStorageStructureHashMap;

/**
    A simple typedef for Map. It is necessary for a more understandable separation of types.
    Represents the internal structure of the DataStorage.
    A string with the name of the key is used as the key. All keys are the same as in DataStorage.
    The value stores a pointer to std::multimap<T, DataStorageRecord*>.
    The key type is same as the DataStorage key value type.
    The value is a pointer to DataStorageRecord.

    Such a complex structure is necessary in order to quickly, in O(log n), 
    find a set of elements that meet a certain requirement, for example, more than a certain value or less than this value
*/
typedef DataMap DataStorageStructureMap;