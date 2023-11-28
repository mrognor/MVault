#pragma once

#include <unordered_map>

#include "DataSaver.h"

/**
    \brief Template prototype of a container for storing different types of data.

    The container should work according to the std::map type, i.e. store a key and value pair in itself.
    Using class specialization, you can define an internal container. Container have to support find(), begin(), end(), emplace(), erase()
    If a pointer is written to one of the elements, then you can set a custom data deletion function.
*/
template <class C>
class DataContainer
{
protected:
    // Container to store all DataSaver's
    C Data;

public:
    // Iterators
    typedef typename C::iterator iterator;
    typedef typename C::const_iterator const_iterator;

    /// Begin provides access to the Data iterator
    inline iterator begin() noexcept { return Data.begin(); }
    /// Cbegin provides access to the Data const_iterator
    inline const_iterator cbegin() const noexcept { return Data.cbegin(); }

    /// End provides access to the Data iterator
    inline iterator end() noexcept { return Data.end(); }
    /// Cend provides access to the Data const_iterator
    inline const_iterator cend() const noexcept { return Data.cend(); }

    /**
        \brief Template method for adding a new data to the container.

        Wrapper over emplace std container

        \param [in] key key for storing the data
        \param [in] data data to store in container
    */
    template <class T>
    void AddData(const std::string& key, const T& data)
    {
        Data.emplace(key, DataSaver(data));
    }

    /**
        \brief Template method for adding a new data to the container and a function to delete this data

        Wrapper over emplace std container
        
        \param [in] key key for storing the data
        \param [in] data data to store in container
        \param [in] deleteFunc function to delete data
    */
    template <class T, class F>
    void AddData(const std::string& key, const T& data, F&& deleteFunc)
    {
        Data.emplace(key, DataSaver(data, deleteFunc));
    }

    /**
        \brief Method for changing the value of a data inside a container using a key

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

        If there was no data with such a key, it will be created

        \param [in] key the key whose value needs to be changed
        \param [in] data new key value
        \param [in] deleteFunc function to delete data
    */
    template <class T, class F>
    void SetData(const std::string& key, const T& data, F&& deleteFunc)
    {
        // Find data iterator
        auto f = Data.find(key);

        // Checking whether there was such a key in the container
        if (f == Data.end())
            AddData(key, data, deleteFunc); // Adding data to the container if there was no key
        else
            f->second.SetData(data, deleteFunc); // Setting new data for the key if it was already in the container
    }

    /**
        Method for getting data from a container using a key

        \param [in] key key for getting data
        \param [out] data a reference to data of type T to write data from the container there.
        If the data was not found, then nothing will be written to the data

        \return Returns false if the key was not found, and otherwise returns true.
    */ 
    template <class T>
    bool GetData(const std::string& key, T& data)
    {
        auto f = Data.find(key);
        if (f == Data.end())
            return false;

        f->second.GetData(data);
        return true;
    }

    /// A method for checking whether data with such a key is in the container
    /// \param [in] key key to find in container
    /// \return Returns false if the key was not found, and otherwise returns true.
    bool IsData(const std::string& key)
    {
        return Data.find(key) != Data.end();
    }

    /**
        \brief Function for erasing data from a container

        If a custom deletion function has been set for the data, it will be called

        \param [in] key key to find in container
    */
    void EraseData(const std::string& key)
    {
        auto f = Data.find(key);
        if (f != Data.end())
        {
            f->second.ResetData();
            Data.erase(f);
        }
    }

    /// Method for clear all data inside container
    /// Note that if pointers were stored in the container, they must be cleaned manually
    void Clear()
    {
        Data.clear();
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
    std::pair<DataContainer::iterator, DataContainer::iterator> GetAllData(const std::string& key)
    {
        return Data.equal_range(key);
    }
};
