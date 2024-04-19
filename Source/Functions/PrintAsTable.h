#pragma once

#include "../VaultRecordRef.h"

namespace mvlt
{
    /**
        \addtogroup VaultFunctions VaultFunctions
        \brief Functions that are available in the library

        @{
    */

    /**
        \brief A method for printing data as tables

        \tparam <T> It takes the std container with VaultRecordRef objects. 

        Not thread safety.

        \param [in] container the std container with the VaultRecordRef`s to be printed
        \param [in] isPrintId will the unique IDs be printed in the table
        \param [in] amountOfRecords The number of records to be printed. The default value is -1, which means that all entries will be output
        \param [in] keys vector of keys to be printed. By default, the vector is empty, which means that all keys will be output
    */ 
    template <template <class E, class Alloc = std::allocator<E>> class T>
    void PrintAsTable(const T<VaultRecordRef>& container, bool isPrintId = false, const std::size_t& amountOfRecords = -1, const std::vector<std::string> keys = {}) noexcept
    {
        if (container.empty()) 
        {
            std::cout << "The keys could not be retrieved because the container is empty" << std::endl;
            std::cout << " (0 records)" << std::endl;
            return;
        }

        std::vector<std::string>* realKeys;
        if (keys.empty())
            realKeys = new std::vector<std::string>(container.front().GetKeys());
        else
            realKeys = const_cast<std::vector<std::string>*>(&keys);

        std::vector<std::size_t> maxLengths(realKeys->size());

        // Fill each max length using key length
        for (std::size_t i = 0; i < realKeys->size(); ++i)
            maxLengths[i] = (*realKeys)[i].length();
        
        // Find each real max length
        std::string dataString;
        for (std::size_t i = 0; i < realKeys->size(); ++i)
        {
            for (const auto& record : container)
            {
                record.GetDataAsString((*realKeys)[i], dataString);
                if (dataString.length() > maxLengths[i]) maxLengths[i] = dataString.length();      
            }
        }

        // Print splitter
        std::cout << "+";
        for (std::size_t i = 0; i < realKeys->size(); ++i)
        {
            std::cout << "-";
            for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "-";
            std::cout << "-+";
        }
        std::cout << std::endl;

        // Print header
        std::cout << "|";
        for (std::size_t i = 0; i < realKeys->size(); ++i)
        {
            std::cout << " " << (*realKeys)[i];
            for (std::size_t j = (*realKeys)[i].length(); j < maxLengths[i]; ++j) std::cout << " ";
            std::cout << " |";
        }
        std::cout << std::endl;

        // Print splitter
        std::cout << "+";
        for (std::size_t i = 0; i < realKeys->size(); ++i)
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
            for (std::size_t i = 0; i < realKeys->size(); ++i)
            {
                record.GetDataAsString((*realKeys)[i], dataString);
                std::cout << " " << dataString;
                for (std::size_t j = dataString.length(); j < maxLengths[i]; ++j) std::cout << " ";
                    std::cout << " |";
            }
            if (isPrintId)
                std::cout << " " << record.GetRecordUniqueId() << std::endl;
            else 
                std::cout << std::endl;

            ++counter;
            if (counter == amountOfRecords) break;
        }

        // Print splitter
        if (amountOfRecords >= container.size())
        {
            std::cout << "+";
            for (std::size_t i = 0; i < realKeys->size(); ++i)
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
            for (std::size_t i = 0; i < realKeys->size(); ++i)
            {
                std::cout << "~";
                for (std::size_t j = 0; j < maxLengths[i]; ++j) std::cout << "~";
                std::cout << "~$";
            }
            std::cout << std::endl;
        }

        std::cout << " (" << container.size() << " records)" << std::endl;

        if (keys.empty())
            delete realKeys;
    }

    /**@} */
}