#pragma once

#include <iostream>

/**
    \brief Class to make real time type check

    Auxiliary class for Data Saver. Required for storing and comparing types inside Data Saver.
*/
class DataTypeSaver
{
private:
    // Type info struct to save type
    const std::type_info& TypeInfo;

public:
    /// A constructor that stores a variable in a class with the type of data stored inside DataSaver
    /// \param [in] type variable with type
    DataTypeSaver(const std::type_info& type);

    /// Method for getting the data type stored inside the class
    /// \return the type of data stored inside the class
    const std::type_info& GetDataType();
};

/**
   \brief A class for storing any type of data

    If a pointer is stored in a class, then you can set a function to automatically clear this pointer when an object of the class is destroyed.

    \warning The class cannot store с-arrays
*/
class DataSaver
{
private:
    // Void pointer to save pointer to any data
    void* Ptr = nullptr;

    // Pointer to data type saver
    DataTypeSaver* DataType = nullptr;

    // Pointer to copy function. Required to DataSaver copy
    void (*CopyFunc)(void*& dst, const void* src) = nullptr;

    // Pointer to delete function. Required to delete pointer since it is not possibly to delete void*
    void (*DeleteFunc)(void*& ptrToDelete) = nullptr;

    // Pointer to custom delete function. Required to delete data if it is pointer
    void (*CustomDeleteFunc)(const void* ptr) = nullptr;

public:
    // Default constructor
    DataSaver();

    /// Copy constructor
    /// \param [in] dataSaver object to be copied
    DataSaver(const DataSaver& dataSaver);

    /// A template constructor that accepts a variable to store inside DataSaver
    /// \param [in] data data to be stored inside DataSaver
    template<class T>
    DataSaver(const T& data)
    {
        SetData(data);
    }

    /**
        \brief A template constructor that accepts a variable and a function to delete a variable

        The constructor allows you to set a function to delete data, 
        which can be convenient when storing pointers, when the pointer type may be unknown, but it must be deleted.

        \param [in] data data to be stored inside the class
        \param [in] customDeleteFunc function to delete data
    */ 
    template<class T, class F>
    DataSaver(const T& data, F&& customDeleteFunc)
    {
        SetData(data, customDeleteFunc);
    }

    /**
        \brief Assignment operator
        
        \param [in] dataSaver object to be copied
        \return returns a new object, with data from dataSaver
    */
    DataSaver& operator=(const DataSaver& dataSaver);
    
    /// Template method to save data inside DataSaver
    /// \param [in] data data to be stored inside the class
    template <class T>
    void SetData(const T& data)
    {   
        SetData(data, nullptr);
    }

    /// Template method to save data and custom delete function inside DataSaver
    /// \param [in] data data to be stored inside the class
    /// \param [in] customDeleteFunc function to delete data
    template <class T, class F>
    void SetData(const T& data, F&& customDeleteFunc)
    {
        // Clear Ptr if it was data before
        if (Ptr != nullptr)
            DeleteFunc(Ptr);

        // Clear DataType if it was data before
        if (DataType != nullptr)
            delete DataType;

        // Create new T type object and save it pointer like void ptr. Data from data will be copying using copy constructor
        Ptr = (void*)new T(data);

        // Create new DataType object to save data type
        DataType = new DataTypeSaver(typeid(data));

        // Set new CopyFunc. It is get to void pointers and convert void pointers to T pointers and copy data.
        CopyFunc = [](void*& dst, const void* src)
            {
                // Convert src pointer to T pointer and get data from T pointer.
                // Use T copy constructor to create T object.
                // Allocate new memory to T type and convert it to void pointer.
                dst = (void*) new T(*(T*)src);
            };

        // Set new DeleteFunc
        DeleteFunc = [](void*& ptrToDelete)
            {
                delete (T*)ptrToDelete;
            };

        // Set custom delete function from dataSaver
        CustomDeleteFunc = customDeleteFunc;
    }

    /// Template method to get data from DataSaver.
    /// \param [out] data the ref to which the data will be written
    /// \return return the true if it successfully recorded the data. If there was no data or they were of a different type it will return false
    template <class T>
    bool GetData(T& data)
    {
        // Check data type stored in DataSaver
        if (DataType != nullptr && DataType->GetDataType() != typeid(data))
        {
            std::cerr << "Wrong type! Was: " + std::string(DataType->GetDataType().name()) + " Requested: " + typeid(data).name() << std::endl;
            return false;
        }

        // Check that was data inside Ptr
        if (Ptr == nullptr)
            return false;

        // Copy data from Ptr to data
        data = *(T*)Ptr;
        return true;
    }

    /// \brief Resets the object to its initial state.
    /// If deleteFunc was set, it will be called
    void ResetData();

    /// Swap data between 2 DataSavers
    /// \param [in, out] dataSaver dataSaver from where the data will be copied to this and where the data from this will be written
    void Swap(DataSaver& dataSaver);

    // Destructor
    ~DataSaver();
};