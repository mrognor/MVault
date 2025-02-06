#include <MVault.h>

/**
    \page CustomTypeExample
    
    # Custom type storage inside the vault
    Since Vault is built on the unordered_multimap from stl, in order to store data inside std::unordered_multimap, 
    a hashing function from stl must be defined for this data type. The comparison operator and the less operator must also be defined.  
    To use the type inside the Vault, you need to specialize the ```struct std::hash<T>``` structure for your type.  
    Inside this function, you need to define an operator() that accepts your type ```std::size_t operator()(const T& dat) const```.  
    You should also specialize ```friend bool operator==(const T& a, const T& b)``` and ```friend bool operator<(const T& a, const T& b)```.

    ## Example for std::vector<int>
    Since the comparison operator and the less operator are defined for the default class, it is not necessary to define them additionally.  
    Let's define the structure for the class of the vector of integers:

    \code{.cpp}
    template <>
    struct std::hash<std::vector<int>>
    {
        std::size_t operator()(const std::vector<int>& vec) const
        {
            std::size_t seed = vec.size();
            for(auto& i : vec)
                seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
    \endcode

    
    ## Example for custom class
    Let's say you need to save the human class in the Vault. There are 3 fields in this class:  
    - Age - integer
    - Name - string
    - Gender - bool

    The class will look like this:
*/

// startcode{.cpp}
class Human
{
private:
    int Age = -1;
    std::string Name = "";
    bool Gender = true;

    friend std::hash<Human>;

    friend bool operator==(const Human& a, const Human& b);

    friend bool operator<(const Human& a, const Human& b);
// insertcode }
// endcode
// ignorecode
public:

    Human() {}

    Human(const int& age, const std::string& name, const bool& gender) : Age(age), Name(name), Gender(gender) {}

    void SetAge(const int& age) noexcept {Age = age;}

    void SetName(const std::string& name) noexcept {Name = name;}

    void SetGender(const bool& gender) noexcept {Gender = gender;}

    int GetAge() const noexcept {return Age;}

    std::string GetName() const noexcept {return Name;}

    bool GetGender() const noexcept {return Gender;}
};
// endignore

/** 
    \page CustomTypeExample
    Then the hashing functionality will look like this:
*/

// startcode{.cpp}
template <>
struct std::hash<Human>
{
    std::size_t operator()(const Human& human) const
    {
        return std::hash<int>()(human.Age) + std::hash<std::string>()(human.Name) + std::hash<bool>()(human.Gender);
    }
};
// endcode

/** 
    \page CustomTypeExample
    Then the operator== will look like this:
*/
// startcode{.cpp}
bool operator==(const Human& a, const Human& b)
{
    if (a.Age == b.Age && a.Name == b.Name && a.Gender == b.Gender) return true;
    else return false;
}
// endcode

/** 
    \page CustomTypeExample
    Then the operator< will look like this:
*/
// startcode{.cpp}
bool operator<(const Human& a, const Human& b)
{
    if (a.Age < b.Age) return true;
    if (a.Age == b.Age)
    {
        if (a.Name < b.Name) return true;
        if (a.Name == b.Name && a.Gender < b.Gender) return true;
    }
    return false;
}
// endcode

/**
    \page CustomTypeExample
    
    We will also define the functionality for printing the human class
*/
// startcode{.cpp}
template <>
std::string mvlt::ToString(const Human& human) noexcept
{
    std::string res = "Name:" + human.GetName() + ", Age:" + std::to_string(human.GetAge()) + ", Gender:";
    if (human.GetGender())
        res += "Man";
    else
        res += "Woman";
    return res; 
}
// endcode

int main()
{
    /**
        \page CustomTypeExample

        ### Usage example
    */
    
    // startcode{.cpp}
    mvlt::Vault vlt;

    vlt.AddKey("Human", Human());

    vlt.CreateRecord({ {"Human", Human(167, "Ciolkovskiy", true)} });
    vlt.CreateRecord({ {"Human", Human(190, "Mendeleev", true)} });
    vlt.CreateRecord({ {"Human", Human(174, "Kovalevskaya", false)} });

    vlt.PrintAsTable();
    // endcode

    /**
        \page CustomTypeExample
        And a result:

        ```bash
        +------------------------------------------+
        | Human                                    |
        +------------------------------------------+
        | Name:Kovalevskaya, Age:174, Gender:Woman |
        | Name:Mendeleev, Age:190, Gender:Man      |
        | Name:Ciolkovskiy, Age:167, Gender:Man    |
        +------------------------------------------+
        (3 records)
        ```
    */
}