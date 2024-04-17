#pragma once

#include "../VaultRecordRef.h"

namespace mvlt
{
    /**
        \brief Function for PrintContainerAsTable

        This function is used as an interface. It is called inside PrintContainerAsTable and allows it to work with VaulRecord and VaultRecordRef and their pointers.

        \param [in] record the pointer to the record
        \param [in] key the key whose value you want to get from the record pointer

        \return the value of the key in the string
    */
    std::string GetDataAsString(VaultRecord* record, const std::string& key) noexcept;

    /**
        \brief Function for PrintContainerAsTable

        This function is used as an interface. It is called inside PrintContainerAsTable and allows it to work with VaulRecord and VaultRecordRef and their pointers.

        \param [in] record the record
        \param [in] key the key whose value you want to get from the record

        \return the value of the key in the string
    */
    std::string GetDataAsString(const VaultRecord& record, const std::string& key) noexcept;

    /**
        \brief Function for PrintContainerAsTable

        This function is used as an interface. It is called inside PrintContainerAsTable and allows it to work with VaulRecord and VaultRecordRef and their pointers.

        \param [in] recordRef the pointer to the VaultRecordRef
        \param [in] key the key whose value you want to get from the VaultRecordRef pointer

        \return the value of the key in the string
    */
    std::string GetDataAsString(VaultRecordRef* recordRef, const std::string& key) noexcept;

    /**
        \brief Function for PrintContainerAsTable

        This function is used as an interface. It is called inside PrintContainerAsTable and allows it to work with VaulRecord and VaultRecordRef and their pointers.

        \param [in] record the recordRef
        \param [in] key the key whose value you want to get from the VaultRecordRef

        \return the value of the key in the string
    */
    std::string GetDataAsString(const VaultRecordRef& recordRef, const std::string& key) noexcept;

    std::string GetUniqueId(VaultRecord* record) noexcept;

    std::string GetUniqueId(const VaultRecord& record) noexcept;

    std::string GetUniqueId(VaultRecordRef* recordRef) noexcept;

    std::string GetUniqueId(const VaultRecordRef& recordRef) noexcept;

    /**
        \brief A method for printing data as tables

        \tparam <T> It takes the std container as the type. The container type can be any type for which the GetDataAsString function is defined

        Not thread safety.

        \param [in] container the container with the data to be printed
        \param [in] isPrintId will the unique IDs be printed in the table
        \param [in] amountOfRecords The number of records to be printed. The default value is -1, which means that all entries will be output
        \param [in] keys vector of keys to be printed. By default, the vector is empty, which means that all keys will be output
    */ 
    template <class T>
    void PrintContainerAsTable(const T& container, bool isPrintId = false, const std::size_t& amountOfRecords = -1, const std::vector<std::string> keys = {}) noexcept
    {
        std::vector<std::size_t> maxLengths(keys.size());

        // Fill each max length using key length
        for (std::size_t i = 0; i < keys.size(); ++i)
            maxLengths[i] = keys[i].length();
        
        // Find each real max length
        for (std::size_t i = 0; i < keys.size(); ++i)
        {
            for (const auto& record : container)
            {
                std::string dataString;
                dataString = GetDataAsString(record, keys[i]);
                if (dataString.length() > maxLengths[i]) maxLengths[i] = dataString.length();      
            }
        }

        // Print splitter
        std::cout << "+";
        for (std::size_t i = 0; i < keys.size(); ++i)
        {
            std::cout << "-";
            for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "-";
            std::cout << "-+";
        }
        std::cout << std::endl;

        // Print header
        std::cout << "|";
        for (std::size_t i = 0; i < keys.size(); ++i)
        {
            std::cout << " " << keys[i];
            for (std::size_t j = keys[i].length(); j < maxLengths[i]; ++j) std::cout << " ";
            std::cout << " |";
        }
        std::cout << std::endl;

        // Print splitter
        std::cout << "+";
        for (std::size_t i = 0; i < keys.size(); ++i)
        {
            std::cout << "-";
            for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "-";
            std::cout << "-+";
        }
        std::cout << std::endl;

        // Print records
        std::size_t counter = 0;
        for (const auto& record : container)
        {
            std::cout << "|";
            for (std::size_t i = 0; i < keys.size(); ++i)
            {
                std::string dataString;
                dataString = GetDataAsString(record, keys[i]);
                std::cout << " " << dataString;
                for (std::size_t j = dataString.length(); j < maxLengths[i]; ++j) std::cout << " ";
                    std::cout << " |";
            }
            if (isPrintId)
                std::cout << " " << GetUniqueId(record) << std::endl;
            else 
                std::cout << std::endl;

            ++counter;
            if (counter == amountOfRecords) break;
        }

        // Print splitter
        if (amountOfRecords >= container.size())
        {
            std::cout << "+";
            for (std::size_t i = 0; i < keys.size(); ++i)
            {
                std::cout << "-";
                for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "-";
                std::cout << "-+";
            }
            std::cout << std::endl;
        }
        else
        {
            std::cout << "$";
            for (std::size_t i = 0; i < keys.size(); ++i)
            {
                std::cout << "~";
                for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "~";
                std::cout << "~$";
            }
            std::cout << std::endl;
        }

        std::cout << " (" << container.size() << " records)" << std::endl;
    }

    /**
        \addtogroup VaultFunctions VaultFunctions
        \brief Functions that are available in the library

        @{
    */


    /**
        \brief Method for printing a container with VaulRecordRefs as table

        \tparam <T> Any container from std containing VaulRecordRefs

        If all VaultRecordRefs inside the container belong to the same Vault, then this method provides thread safety

        \param [in] containerWithRefs Container with VaulRecordRefs
        \param [in] isPrintId will the unique IDs be printed in the table
        \param [in] amountOfRecords The number of records to be printed. The default value is -1, which means that all entries will be output
        \param [in] keys vector of keys to be printed. By default, the vector is empty, which means that all keys will be output
    */
    template <template <class E, class Alloc = std::allocator<E>> class T>
    void PrintAsTable(const T<VaultRecordRef>& containerWithRefs, bool isPrintId = false, const std::size_t& amountOfRecords = -1, const std::vector<std::string> keys = {}) noexcept
    {
        if (containerWithRefs.empty()) 
        {
            std::cout << "The keys could not be retrieved because the container is empty" << std::endl;
            std::cout << " (0 records)" << std::endl;
            return;
        }

        containerWithRefs.begin()->ReadLock();
        if (keys.empty())
            PrintContainerAsTable(containerWithRefs, isPrintId, amountOfRecords, containerWithRefs.begin()->GetKeys());
        else
            PrintContainerAsTable(containerWithRefs, isPrintId, amountOfRecords, keys);            
        containerWithRefs.begin()->ReadUnlock();        
    }

    /**@} */
}