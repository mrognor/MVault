#pragma once

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "DataSaver.h"

namespace mvlt
{
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
        void AddData(const std::string& key, const T& data) noexcept
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
        void AddData(const std::string& key, const T& data, F&& deleteFunc) noexcept
        {
            Container.emplace(key, DataSaver(data, deleteFunc));
        }

        /**
            \brief Method for adding a new data to the container

            \param [in] key the key whose value needs to be changed
            \param [in] dataSaver dataSaver with any type, the value from which will be copied to the container 
        */
        void AddDataFromDataSaver(const std::string& key, const DataSaver& dataSaver) noexcept
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
        void SetData(const std::string& key, const T& data) noexcept
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
        void SetData(const std::string& key, const T& data, F&& deleteFunc) noexcept
        {
            // Find data iterator
            auto findResIt = Container.find(key);

            // Checking whether there was such a key in the container
            if (findResIt == Container.end())
                AddData(key, data, deleteFunc); // Adding data to the container if there was no key
            else
                findResIt->second.SetData(data, deleteFunc); // Setting new data for the key if it was already in the container
        }

        /**
            \brief Method for changing the value of a data inside a container using a key

            If there was no data with such a key, it will be created

            \param [in] key the key whose value needs to be changed
            \param [in] dataSaver dataSaver with any type, the value from which will be copied to the container 
        */
        void SetDataFromDataSaver(const std::string& key, const DataSaver& dataSaver) noexcept
        {
            // Find data iterator
            auto findResIt = Container.find(key);

            // Checking whether there was such a key in the container
            if (findResIt == Container.end())
                AddDataFromDataSaver(key, dataSaver); // Adding data to the container if there was no key
            else
                findResIt->second = dataSaver; // Setting new data for the key if it was already in the container
        }

        /**
            \brief Method for setting data by key

            \param [in] key the key to set the data on
            \param [in] str the string that will be converted to data
        */
        void SetDataFromString(const std::string& key, const std::string& str) noexcept
        {
            // Find data iterator
            auto findResIt = Container.find(key);

            // Checking whether there was such a key in the container
            if (findResIt != Container.end())
                findResIt->second.SetDataFromString(str);
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
        bool GetData(const std::string& key, T& data) const noexcept
        {
            auto findResIt = Container.find(key);
            if (findResIt == Container.end())
                return false;

            findResIt->second.GetData(data);
            return true;
        }

        /**
            \brief Method for getting dataSaver from a container using a key

            \param [in] key key for getting data
            \param [out] dataSaver a reference to dataSaver to write data from the container there.
            If the data was not found, then nothing will be written to the dataSaver

            \return Returns false if the key was not found, and otherwise returns true.
        */ 
        bool GetDataSaver(const std::string& key, DataSaver& dataSaver) const noexcept
        {
            auto findResIt = Container.find(key);
            if (findResIt == Container.end())
                return false;

            dataSaver = findResIt->second;
            return true;
        }
        
        /**
            \brief Method for getting data converted to string from a container using a key

            \param [in] key key for getting data
            \param [out] str a reference to string to write data from the container there.
            If the data was not found, then nothing will be written to the str

            \return Returns false if the key was not found, and otherwise returns true.
        */ 
        bool GetDataAsString(const std::string& key, std::string& str) const noexcept
        {
            auto findResIt = Container.find(key);
            if (findResIt == Container.end())
                return false;

            str = findResIt->second.Str();
            return true;
        }

        /// \brief A method for checking whether data with such a key is in the container
        /// \param [in] key key to find in container
        /// \return Returns false if the key was not found, and otherwise returns true.
        bool IsData(const std::string& key) const noexcept
        {
            return Container.find(key) != Container.end();
        }

        /**
            \brief Function for erasing data from a container

            If a custom deletion function has been set for the data, it will be called

            \param [in] key key to find in container
        */
        void EraseData(const std::string& key) noexcept
        {
            auto findResIt = Container.find(key);
            if (findResIt != Container.end())
            {
                findResIt->second.ResetData();
                Container.erase(findResIt);
            }
        }

        /// \brief Method for clear all data inside container
        /// Note that if pointers were stored in the container, they must be cleaned manually
        void Clear() noexcept
        {
            Container.clear();
        }

        /// \brief Method for getting the container size
        /// \return Container size
        std::size_t Size() const noexcept
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
        std::pair<DataContainer::const_iterator, DataContainer::const_iterator> GetAllData(const std::string& key) const noexcept
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
        std::pair<DataContainer::const_iterator, DataContainer::const_iterator> GetAllData(const std::string& key) const noexcept
        {
            return Container.equal_range(key);
        }
    };
}