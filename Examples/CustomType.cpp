/*
    This file demonstrates the use of a custom data type.
*/

#include <MVault.h>

// Define class
class Human
{
private:
    int Age = -1;
    std::string Name = "";
    bool Gender = true;

    friend std::hash<Human>;

    friend bool operator==(const Human& a, const Human& b);

    friend bool operator<(const Human& a, const Human& b);
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

// Specialize std::hash for custom class
template <>
struct std::hash<Human>
{
    std::size_t operator()(const Human& human) const
    {
        return std::hash<int>()(human.Age) + std::hash<std::string>()(human.Name) + std::hash<bool>()(human.Gender);
    }
};

// Specialize compare operator for custom class
bool operator==(const Human& a, const Human& b)
{
    if (a.Age == b.Age && a.Name == b.Name && a.Gender == b.Gender) return true;
    else return false;
}

// Specialize less operator for custom class
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

// Specialize ToString function for custom class
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

int main()
{
    mvlt::Vault vlt;

    vlt.AddKey("Human", Human());

    vlt.CreateRecord({ {"Human", Human(41, "Alexei Ivanov", true)} });
    vlt.CreateRecord({ {"Human", Human(25, "Maria Rodriguez", false)} });
    vlt.CreateRecord({ {"Human", Human(74, "Miguel Silva", true)} });

    vlt.Print();
}