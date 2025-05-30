#pragma once

#include <map>
#include <unordered_map>

namespace mvlt 
{
    /**
        \brief Iterator class for all library maps

        \tparam <MultiContainerIterator> - The iterator type for the multi container
        \tparam <ContainerIterator> - The type of iterator for the container
        \tparam <KeyType> - The type of key for the map
        \tparam <ValueType> - The type of value for the map
    */
    template <class MultiContainerIterator, class ContainerIterator, class KeyType, class ValueType>
    class AllMapIterator
    {
    private:
        // Is this iterator for multi container
        bool IsMultiMap;

        // Multi container iterator
        MultiContainerIterator DataMultiMapIt;

        // Container iterator
        ContainerIterator DataMapIt;
    public:

        /// \brief Default constructor
        AllMapIterator() noexcept = default;

        /**
            \brief Copy constructor

            \param [in] other object to copy data from
        */
        AllMapIterator(const AllMapIterator& other) noexcept
        {
            IsMultiMap = other.IsMultiMap;
            DataMultiMapIt = other.DataMultiMapIt;
            DataMapIt = other.DataMapIt;
        }

        /**
            \brief Deleted move constructor

            \param [in] other object to copy data from
        */
        AllMapIterator(AllMapIterator&& other) noexcept
        {
            IsMultiMap = other.IsMultiMap;
            DataMultiMapIt = other.DataMultiMapIt;
            DataMapIt = other.DataMapIt;
        }

        /**
            \brief Constructor

            \param [in] dataMultiMapIt iterator to multi container
            \param [in] dataMapIt iterator to container
            \param [in] isMultiMap is this Map multi 
        */
        AllMapIterator(const MultiContainerIterator& dataMultiMapIt,
            const ContainerIterator& dataMapIt, const bool& isMultiMap) noexcept
        {
            IsMultiMap = isMultiMap;
            DataMultiMapIt = dataMultiMapIt;
            DataMapIt = dataMapIt;
        };

        /**
            \brief Assignment operator

            \param [in] other object to copy data from

            \return assigned object
        */
        AllMapIterator& operator=(const AllMapIterator& other) noexcept
        {
            if (&other != this)
            {
                IsMultiMap = other.IsMultiMap;
                DataMultiMapIt = other.DataMultiMapIt;
                DataMapIt = other.DataMapIt;
            }

            return *this;
        }

        /**
            \brief Deleted move assignment operator

            \param [in] other object to copy data from

            \return assigned object
        */
        AllMapIterator& operator=(AllMapIterator&& other) noexcept
        {
            if (&other != this)
            {
                IsMultiMap = other.IsMultiMap;
                DataMultiMapIt = other.DataMultiMapIt;
                DataMapIt = other.DataMapIt;
            }

            return *this;
        }

        /**
            \brief Not equal operator

            \param [in] other object to compare

            \return true it other and this not equal
        */
        bool operator!=(const AllMapIterator& other) const noexcept
        { 
            if (IsMultiMap)
                return DataMultiMapIt != other.DataMultiMapIt;
            else
                return DataMapIt != other.DataMapIt;
        }

        /**
            \brief Compare operator

            \param [in] other object to compare

            \return true it other and this equal
        */
        bool operator==(const AllMapIterator& other) const noexcept
        {
            if (IsMultiMap)
                return DataMultiMapIt == other.DataMultiMapIt;
            else
                return DataMapIt == other.DataMapIt;
        }

        /**
            \brief Pre increment operator

            \return incremented iterator
        */
        AllMapIterator& operator++() noexcept
        {
            if (IsMultiMap)
                ++DataMultiMapIt;
            else
                ++DataMapIt;
            return *this;
        }

        /**
            \brief Pre decrement operator

            \return decremented iterator
        */
        AllMapIterator& operator--() noexcept
        {
            if (IsMultiMap)
                --DataMultiMapIt;
            else
                --DataMapIt;
            return *this;
        }

        /**
            \brief Operator*

            \return pair with iterator key and value
        */
        std::pair<KeyType, ValueType> operator*() const noexcept
        {
            if (IsMultiMap)
                return *DataMultiMapIt;
            else
                return *DataMapIt;
        }

        /**
            \brief Method for get container iterator

            \return stl container iterator
        */
        ContainerIterator GetContainerIterator() const noexcept
        {
            return DataMapIt;
        }

        /**
            \brief Method for get multi container iterator

            \return stl multi container iterator
        */
        MultiContainerIterator GetMultiContainerIterator() const noexcept
        {
            return DataMultiMapIt;
        }

        /// \brief Default destructor
        ~AllMapIterator() = default;
    };

    /**
        \brief Declaring the UnorderedMapIterator class
    */
    template <class KeyType, class ValueType>
    using UnorderedMapIterator = AllMapIterator<typename std::unordered_multimap<KeyType, ValueType>::iterator, 
        typename std::unordered_map<KeyType, ValueType>::iterator, KeyType, ValueType>;

    /**
        \brief Declaring the MapIterator class
    */
    template <class KeyType, class ValueType>
    using MapIterator = AllMapIterator<typename std::multimap<KeyType, ValueType>::iterator, 
        typename std::map<KeyType, ValueType>::iterator, KeyType, ValueType>;

    /**
        \brief Declaring the ReverseMapIterator class
    */
    template <class KeyType, class ValueType>
    using ReverseMapIterator = AllMapIterator<typename std::multimap<KeyType, ValueType>::reverse_iterator, 
        typename std::map<KeyType, ValueType>::reverse_iterator, KeyType, ValueType>;


    /**
        \brief Parent wrapper class for stl containers

        This class is needed to provide the same interface to std::unordered_map and std::unordered_multimap, 
        and for std::map and std::multimap. At one point in time, this class stores data either in a multi-container or in a regular one.

        \tparam <MultiContainer> - A class for a multi container
        \tparam <Container> - A class for a container
        \tparam <Iterator> - A class for a ParentMap iterator
        \tparam <KeyType> - A class for a container and multi container key type
        \tparam <ValueType> - A class for a container and multi container value type
    */
    template <class MultiContainer, class Container, class Iterator, class KeyType, class ValueType>
    class ParentMap
    {
    protected:
        /// Is this multi map
        bool IsMultiMap = false;

        /// Multi container
        MultiContainer DataMultiMap;

        /// Container
        Container DataMap;
    public:

        /**
            \brief Begin container method

            The need for the container to support foreach loops

            \return Firtst map container iterator
        */
        Iterator begin() noexcept
        {
            return Iterator(DataMultiMap.begin(), DataMap.begin(), IsMultiMap);
        }

        /**
            \brief End container method

            The need for the container to support foreach loops

            \return Last map container iterator
        */
        Iterator end() noexcept
        {
            return Iterator(DataMultiMap.end(), DataMap.end(), IsMultiMap);
        }

        /**
            \brief Begin container method

            \return Firtst map container iterator
        */
        Iterator Begin() noexcept
        {
            return Iterator(DataMultiMap.begin(), DataMap.begin(), IsMultiMap);
        }

        /**
            \brief End container method

            \return Last map container iterator
        */
        Iterator End() noexcept
        {
            return Iterator(DataMultiMap.end(), DataMap.end(), IsMultiMap);
        }

        /**
            \brief The method to check if it is multi container

            \return true if it is multicontiner, otherwise return false
        */
        bool IsMultiContainer() const noexcept
        {
            return IsMultiMap;
        }

        /**
            \brief The method for inserting data into the Map

            \tparam <EmplaceKeyType> - Key type
            \tparam <EmplaceValueType> - Value type

            \param [in] key rvalue to key
            \param [in] value rvlaue to value

            \return A pair consisting of an iterator to the inserted element 
            (or to the element that prevented the insertion) and a bool value set to true if and only if the insertion took place.
        */
        template <class EmplaceKeyType, class EmplaceValueType>
        std::pair<Iterator, bool> Emplace(EmplaceKeyType&& key, EmplaceValueType&& value) noexcept
        {
            if (IsMultiMap)
            {
                auto dataMultiMapIt = DataMultiMap.emplace(std::forward<EmplaceKeyType>(key), std::forward<EmplaceValueType>(value));
                return std::pair<Iterator, bool>(Iterator(dataMultiMapIt, DataMap.end(), true), true);
            }
            else
            {
                auto dataMapIt = DataMap.emplace(std::forward<EmplaceKeyType>(key), std::forward<EmplaceValueType>(value));
                return std::pair<Iterator, bool>(Iterator(DataMultiMap.end(), dataMapIt.first, false), dataMapIt.second);
            }
        }

        /**
            \brief The method for getting range of data

            \param [in] data key to find

            \return Pair with first and last iterator with key equals to data
        */
        std::pair<Iterator, Iterator> EqualRange(const KeyType& data) noexcept
        {
            if (IsMultiMap)
            {
                auto rangePair = DataMultiMap.equal_range(data);
                return std::pair<Iterator, Iterator>(Iterator(rangePair.first, DataMap.end(), true), Iterator(rangePair.second, DataMap.end(), true));
            }
            else 
            {
                auto rangePair = DataMap.equal_range(data);
                return std::pair<Iterator, Iterator>(Iterator(DataMultiMap.end(), rangePair.first, false), Iterator(DataMultiMap.end(), rangePair.second, false));
            }
        }

        /**
            \brief The method for getting iterator with key

            \param [in] data key to find

            \return Iterator with key equals to data
        */
        Iterator Find(const KeyType& data) noexcept
        {
            if (IsMultiMap)
            {
                auto retIt = DataMultiMap.find(data);
                return Iterator(retIt, DataMap.end(), true);
            }
            else
            {
                auto retIt = DataMap.find(data);
                return Iterator(DataMultiMap.end(), retIt, false);
            }
        }

        /**
            \brief The method for erase data from Map with key

            \param [in] data key to erase

            \return Number of deleted items
        */
        std::size_t Erase(const KeyType& data) noexcept
        {
            std::size_t res;
            if (IsMultiMap)
                res = DataMultiMap.erase(data);
            else
                res = DataMap.erase(data);
            
            return res;
        }

        /**
            \brief The method for erase data from Map with iterator

            \param [in] dataIt iterator to erase

            \return Iterator following the last removed element.
        */
        Iterator Erase(const Iterator& dataIt) noexcept
        {
            if (IsMultiMap)
            {
                auto retIt = DataMultiMap.erase(dataIt.GetMultiContainerIterator());
                return Iterator(retIt, DataMap.end(), true);
            }
            else
            {
                auto retIt = DataMap.erase(dataIt.GetContainerIterator());
                return Iterator(DataMultiMap.end(), retIt, false);
            }
        }

        /// \brief The method for clear Map
        void Clear() noexcept
        {
            if (IsMultiMap)
                DataMultiMap.clear();
            else
                DataMap.clear();
        }

        /**
            \brief The method for get Map size

            \return Map size
        */
        std::size_t Size() noexcept
        {
            if (IsMultiMap)
                return DataMultiMap.size();
            else
                return DataMap.size();
        }
    };

    /**
        \brief The descendant of the ParentMap class, which stores the map, i.e. the binary tree
    */
    template <class KeyType, class ValueType>
    class Map : public ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, 
        MapIterator<KeyType, ValueType>, KeyType, ValueType>
    {
    public:
        /**
            \brief Class constructor

            \param [in] isMultiMap is this UnorderedMap store data in std::multimap
        */
        explicit Map(const bool& isMultiMap) noexcept
        {
            ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::IsMultiMap = isMultiMap;
        }

        /**
            \brief Returns an iterator pointing to the first element that is not less than (i.e. greater or equal to) data

            \param [in] data lower bound

            \return Iterator pointing to the first element that is not less than key. If no such element is found, a End() is returned.

        */
        MapIterator<KeyType, ValueType> LowerBound(const KeyType& data) noexcept
        {
            return MapIterator<KeyType, ValueType>(ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::DataMultiMap.lower_bound(data), 
                ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::DataMap.lower_bound(data),
                ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::IsMultiMap);
        }

        /**
            \brief Returns an iterator pointing to the first element that is greater than key.

            \param [in] data lower bound

            \return Iterator pointing to the first element that is greater than key. If no such element is found, End() iterator is returned.

        */
        MapIterator<KeyType, ValueType> UpperBound(const KeyType& data) noexcept
        {
            return MapIterator<KeyType, ValueType>(ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::DataMultiMap.upper_bound(data), 
                ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::DataMap.upper_bound(data),
                ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::IsMultiMap);
        }

        /**
            \brief Reverse begin container method

            \return Last map container iterator
        */
        ReverseMapIterator<KeyType, ValueType> Rbegin() noexcept
        {
            return ReverseMapIterator<KeyType, ValueType>(ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::DataMultiMap.rbegin(), 
                ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::DataMap.rbegin(),
                ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::IsMultiMap);
        }

        /**
            \brief Reverse end container method

            \return First map container iterator
        */
        ReverseMapIterator<KeyType, ValueType> Rend() noexcept
        {
            return ReverseMapIterator<KeyType, ValueType>(ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::DataMultiMap.rend(), 
            ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::DataMap.rend(), 
            ParentMap<std::multimap<KeyType, ValueType>, std::map<KeyType, ValueType>, MapIterator<KeyType, ValueType>, KeyType, ValueType>::IsMultiMap);
        }
    };

    /**
        \brief The descendant of the ParentMap class, which stores the unordered_map, i.e. the hash table
    */
    template <class KeyType, class ValueType>
    class UnorderedMap : public ParentMap<std::unordered_multimap<KeyType, ValueType>, std::unordered_map<KeyType, ValueType>, 
        UnorderedMapIterator<KeyType, ValueType>, KeyType, ValueType>
    {
    public:
        /**
            \brief Class constructor

            \param [in] isMultiMap is this UnorderedMap store data in std::unordered_multimap
        */
        explicit UnorderedMap(const bool& isMultiMap) noexcept
        {
            ParentMap<std::unordered_multimap<KeyType, ValueType>, std::unordered_map<KeyType, ValueType>, UnorderedMapIterator<KeyType, ValueType>, KeyType, ValueType>::IsMultiMap = isMultiMap;
        }
    };
}