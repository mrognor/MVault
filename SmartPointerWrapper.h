#pragma once

/**
    \brief A class for storing pointers.
    
    It stores a pointer inside itself, so when copying or assigning objects of this class, 
    the pointer will be copied, but the contents will be the same. The memory for the pointer will be allocated only when the object is created, 
    no memory will be allocated when copying or assigning. The deletion of the pointer will occur when the last object of the class storing the pointer is destroyed. 
    A counter is implemented inside the class, which increases when copying or assigning an object, and decreases when deleting or re-assigning
*/
template <class T>
class SmartPointerWrapper
{
private:
    // Pointer to store data inside wrapper
    T* Data = nullptr;
    // Pointer to store ref counter
    std::size_t* RefCounter = nullptr;
public:

    /// \brief Copy constructor
    /// \param [in] other object to be copied
    SmartPointerWrapper(const SmartPointerWrapper& other)
    {
        *this = other;
    }

    /// \brief Copy constructor
    /// \param [in] other object to be copied
    SmartPointerWrapper(const SmartPointerWrapper&& other)
    {
        *this = other;
    }

    /// \brief Copy constructor
    /// \param [in] other object to be copied
    SmartPointerWrapper(SmartPointerWrapper& other)
    {
        *this = other;
    }

    /// \brief Copy constructor
    /// \param [in] other object to be copied
    SmartPointerWrapper(SmartPointerWrapper&& other)
    {
        *this = other;
    }

    /**
        \brief A template constructor that accepts a parameter package and passes it to a constructor of a specialized type

        \param [in] args A package of parameters to pass to the constructor of the internal type.

        This constructor allows you to pass the parameters necessary to create an object of class T. Inside, the constructor looks like this:

        \code
            Data = new T(args...);
        \endcode

        For example, let the SmartPointerWrapper store a int in itself, 
        and we want to assign it the value when creating an object, 
        for this we will create an object of the SmartPointerWrapper class with int as a template specialization and 1 in constructor param.

        \code
            SmartPointerWrapper<int> pointer(1);
        \endcode

        This constructor will get one int type parameter with a value of 1 and create an int type pointer with a value of 1.

        \code
            Data = new int(1);
        \endcode

        If a more complex object is stored inside the SmartPointerWrapper, for example, 
        a class for storing coordinates, the constructor of which takes 2 int parameters, 
        then you can create an object of the SmartPointerWrapper class as follows:

        \code
            SmartPointerWrapper<pointd2d> pointer(1, 1);
        \endcode

        And inside the constructor it will look like this:
        
        \code
            Data = new pointd2d(1, 1);
        \endcode
    */
    template <class... Args>
    SmartPointerWrapper(Args&&... args)
    {
        Data = new T(args...);
        RefCounter = new std::size_t(1);
    }

    /**
        \brief Assignment operator
        
        \param [in] other object to be copied
        \return returns a new object, with data from other
    */
    SmartPointerWrapper& operator=(const SmartPointerWrapper& other)
    {
        // Check is other same as this
        if (&other != this)
        {
            // Checking whether the data was previously assigned
            if (RefCounter != nullptr)
            {
                // Destroy previously data if this object was last
                if (*RefCounter == 1)
                {
                    delete RefCounter;
                    delete Data;
                }
                else
                {
                    --*RefCounter;
                }
            }

            Data = other.Data;
            RefCounter = other.RefCounter;
            ++*RefCounter;
        }

        return *this;
    }

    /// \brief Method for assigning a value to a pointer
    /// \param [in] data Data to copy to the pointer
    void SetData(const T& data)
    {
        *Data = data;
    }

    /// \brief Data getter
    /// \return returns the data stored in the class
    T GetData()
    {
        return *Data;
    }

    /// \brief Default destructor
    ~SmartPointerWrapper()
    {
        // Check if it not empty wrapper
        if(RefCounter != nullptr)
        {
            // Check reference counter
            if (*RefCounter == 1)
            {
                delete RefCounter;
                delete Data;
            }
            else
            {
                --*RefCounter;
            }
        }
    }
};