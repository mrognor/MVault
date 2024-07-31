#pragma once

#include <string>
#include <iostream>
#include <typeindex>

#include "Functions/ToString.h"
#include "Functions/FromString.h"

namespace mvlt
{
    /**
        \brief A class for storing any type of data

        If a pointer is stored in a class, then you can set a function to automatically clear this pointer when an object of the class is destroyed.
        By default, it stores the void type.

        \warning The class cannot store с-arrays
    */
    class DataSaver
    {
    private:
        // Void pointer to save pointer to any data
        void* Ptr = nullptr;

        // Pointer to data type saver
        std::type_index DataType;

        // Pointer to copy function. Required to DataSaver copy
        void (*CopyFunc)(void*& dst, void* src) = nullptr;

        // Pointer to delete function. Required to delete pointer since it is not possibly to delete void*
        void (*DeleteFunc)(void*& ptrToDelete) = nullptr;

        // A pointer to a function that will convert the value stored inside to a string
        std::string (*ToStringFunc)(void* ptrToPrint) = nullptr;

        // Pointer to custom delete function. Required to delete data if it is pointer
        void (*CustomDeleteFunc)(const void* ptr) = nullptr;

        // Pointer to function to load data from string to data saver
        bool (*SetDataFromStringFunc)(void* ptrToStoreDataFromString, const std::string& str) = nullptr;

    public:
        /// \brief Default constructor
        DataSaver() noexcept;

        /**
            \brief Copy constructor

            \param [in] dataSaver object to be copied
        */
        DataSaver(const DataSaver& dataSaver) noexcept;

        /**
            \brief A template constructor that accepts a variable to store inside DataSaver

            \tparam <T> Any type of data except for c arrays

            \param [in] data data to be stored inside DataSaver
        */
        template<class T>
        DataSaver(const T& data) noexcept : DataType(typeid(void))
        {
            SetData(data);
        }

        /**
            \brief A template constructor that accepts a variable and a function to delete a variable

            \tparam <T> Any type of data except for c arrays
            \tparam <F> Function pointer or lambda function

            The constructor allows you to set a function to delete data, 
            which can be convenient when storing pointers, when the pointer type may be unknown, but it must be deleted.

            \param [in] data data to be stored inside the class
            \param [in] customDeleteFunc function to delete data
        */ 
        template<class T, class F>
        DataSaver(const T& data, F&& customDeleteFunc) noexcept : DataType(typeid(void))
        {
            SetData(data, customDeleteFunc);
        }

        /**
            \brief Assignment operator
            
            \param [in] dataSaver object to be copied
            \return returns a new object, with data from dataSaver
        */
        DataSaver& operator=(const DataSaver& dataSaver) noexcept;
        
        /**
            \brief Template method to save data inside DataSaver

            \tparam <T> Any type of data except for c arrays

            \param [in] data data to be stored inside the class
        */
        template <class T>
        void SetData(const T& data) noexcept
        {   
            SetData(data, nullptr);
        }

        /**
            \brief A method for saving data from a string to a DataStorage

            \param [in] data a string with data
        */
        bool SetDataFromString(const std::string& data) noexcept;

        /**
            \brief Template method to save data and custom delete function inside DataSaver

            \tparam <T> Any type of data except for c arrays
            \tparam <F> Function pointer or lambda function

            \param [in] data data to be stored inside the class
            \param [in] customDeleteFunc function to delete data
        */
        template <class T, class F>
        void SetData(const T& data, F&& customDeleteFunc) noexcept
        {
            // Clear Ptr if it was data before
            if (Ptr != nullptr)
                DeleteFunc(Ptr);

            // Create new T type object and save it pointer like void ptr. Data from data will be copying using copy constructor
            Ptr = static_cast<void*>(new T(data));

            // Save data type to DataType
            DataType = typeid(data);

            // Set new CopyFunc. It is get to void pointers and convert void pointers to T pointers and copy data.
            CopyFunc = [](void*& dst, void* src)
                {
                    // Convert src pointer to T pointer and get data from T pointer.
                    // Use T copy constructor to create T object.
                    // Allocate new memory to T type and convert it to void pointer.
                    dst = static_cast<void*>(new T(*static_cast<T*>(const_cast<void*>(src))));
                };

            // Set new DeleteFunc
            DeleteFunc = [](void*& ptrToDelete)
                {
                    delete static_cast<T*>(ptrToDelete);
                };

            // Set new to string function
            ToStringFunc = [](void* ptrToPrint)
                {
                    return ToString(*static_cast<T*>(ptrToPrint));
                };

            // Set custom delete function from dataSaver
            CustomDeleteFunc = customDeleteFunc;

            // Set new copy from string function
            SetDataFromStringFunc = [](void* ptrToStoreDataFromString, const std::string& str)
                {
                    return FromString(str, *static_cast<T*>(ptrToStoreDataFromString));
                };
        }

        /**
            \brief Template method to get data from DataSaver.
        
            \tparam <T> Any type of data except for c arrays

            \param [out] data the ref to which the data will be written

            \return return the true if it successfully recorded the data. 
            If there was no data or they were of a different type it will return false
        */
        template <class T>
        bool GetData(T& data) const noexcept
        {
            // Check data type stored in DataSaver
            if (DataType != typeid(data))
            {
                std::cerr << "Wrong type! Was: " + std::string(DataType.name()) + " Requested: " + typeid(data).name() << std::endl;
                return false;
            }

            // Check that was data inside Ptr
            if (Ptr == nullptr)
                return false;

            // Copy data from Ptr to data
            data = *static_cast<T*>(Ptr);
            return true;
        }

        /**
            \brief Resets the object to its initial state.

            If deleteFunc was set, it will be called
        */
        void ResetData() noexcept;

        /**
            \brief Swap data between 2 DataSavers

            \param [in, out] dataSaver dataSaver from where the data will be copied to this and where the data from this will be written
        */
        void Swap(DataSaver& dataSaver) noexcept;

        /**
            \brief A method for getting a string that represents data inside a class object

            Used ToString function.

            \return A string of data
        */
        std::string Str() const noexcept;

        /**
            \brief A method for getting the type of saved data.

            \return std::type_index object with saved data type
        */
        std::type_index GetDataType() const noexcept;

        /// \brief Default destructor
        ~DataSaver() noexcept;
    };
}