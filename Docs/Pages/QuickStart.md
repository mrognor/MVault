# Quick start
## Installing the library
1. Clone MVault [repository](https://github.com/mrognor/MVault).  
To do this, you can use the command:
```bash
git clone https://github.com/mrognor/MVault.git
```

2. Go to the *Scripts* folder and call the script *MakeRelease.sh*.  
After that, the *MVault* folder with the library will be created in the *Release* directory.  
Copy this folder to your project folder or to any place convenient for you.

3. To build a project using the library, let's write a simple example and compile it
```cpp
// main.cpp
#include "MVault.h"

int main()
{
    mvlt::Vault vlt;

    vlt.AddKey("A", -1);
    vlt.AddKey("B", -1);
    
    for (int i = 0; i < 10; ++i)
        vlt.CreateRecord({ {"A", i}, {"B", 10 - i} });

    vlt.PrintAsTable();
}
```
To build it, use the command:
```bash
g++ -Wall main.cpp -I/path/to/include/dir -L/path/to/bin/dir -lMVault -lpthread
```
Don't forget to change the /path/to/include/dir and /path/to/bin/dir and link pthread.  
The output of the program should be as follows:
```bash
+---+----+
| A | B  |
+---+----+
| 9 | 1  |
| 8 | 2  |
| 7 | 3  |
| 6 | 4  |
| 5 | 5  |
| 4 | 6  |
| 2 | 8  |
| 1 | 9  |
| 3 | 7  |
| 0 | 10 |
+---+----+
 (10 records)
```

## Code example
Let's create a simple project that demonstrates the main features of the library.
Before you start, you need to create an Enum to determine the nationality and create a function to display it on the screen.  
```cpp
enum class Nationality
{
    Earth,
    Russia,
    China,
    Spain,
    Britain
};

template <>
std::string mvlt::ToString(const Nationality& nation) 
{
    switch (nation) 
    {
        case Nationality::Earth:
            return "Cosmopolitan";
        case Nationality::Russia:
            return "Russia";
        case Nationality::China:
            return "China";
        case Nationality::Spain:
            return "Spain";
        case Nationality::Britain:
            return "Britain";
    }
    return "";
}
```

Also, for example, the function of printing variables of the bool type has been changed. 
It is defined in the template in <em> .h </em> the file and therefore it can be changed to a custom one, 
which is done to display the gender of a person. The function is located in the folder *Functions* in *include*.

Let's create a Vault and add 5 keys to it:
```cpp
mvlt::Vault humans;

humans.AddKey("Name", std::string(""));
humans.AddKey("Gender", true);
humans.AddKey("Age", -1);
humans.AddKey("Nationality", Nationality::Earth); 
```

It is important not to forget that by default "" in c++ is an array of chars, so you need to wrap it in *std::string*.  
Let's create 10 records:  

```cpp
humans.CreateRecord({ {"Name", std::string("Anton")}, {"Gender", true}, {"Age", 32}, {"Nationality", Nationality::Russia} });
humans.CreateRecord({ {"Name", std::string("Artyom")}, {"Gender", true}, {"Age", 46}, {"Nationality", Nationality::Russia} });
humans.CreateRecord({ {"Name", std::string("Oksana")}, {"Gender", false}, {"Age", 18}, {"Nationality", Nationality::Russia} });
humans.CreateRecord({ {"Name", std::string("Muchen")}, {"Gender", true}, {"Age", 97}, {"Nationality", Nationality::China} });
humans.CreateRecord({ {"Name", std::string("Yinuo")}, {"Gender", false}, {"Age", 21}, {"Nationality", Nationality::China} });
humans.CreateRecord({ {"Name", std::string("Maria")}, {"Gender", false}, {"Age", 37}, {"Nationality", Nationality::Spain} });
humans.CreateRecord({ {"Name", std::string("Huan")}, {"Gender", true}, {"Age", 48}, {"Nationality", Nationality::Spain} });
humans.CreateRecord({ {"Name", std::string("John")}, {"Gender", true}, {"Age", 65}, {"Nationality", Nationality::Britain} });
humans.CreateRecord({ {"Name", std::string("Diana")}, {"Gender", false}, {"Age", 26}, {"Nationality", Nationality::Britain} });
humans.CreateRecord({ {"Name", std::string("Baro")}, {"Gender", true}, {"Age", 28}, {"Nationality", Nationality::Earth} });
```

You can display records on the screen using the *PrintAsTable* function.

```cpp
humans.PrintAsTable();
```

And get the following result:
```bash
+-----+--------+--------+--------------+
| Age | Gender | Name   | Nationality  |
+-----+--------+--------+--------------+
| 28  | Male   | Baro   | Cosmopolitan |
| 26  | Female | Diana  | Britain      |
| 48  | Male   | Huan   | Spain        |
| 37  | Female | Maria  | Spain        |
| 21  | Female | Yinuo  | China        |
| 97  | Male   | Muchen | China        |
| 18  | Female | Oksana | Russia       |
| 46  | Male   | Artyom | Russia       |
| 65  | Male   | John   | Britain      |
| 32  | Male   | Anton  | Russia       |
+-----+--------+--------+--------------+
 (10 records)
```

Let's make a complex query, for example, we will ask all Russian or British men over the age of 35:

```cpp
mvlt::VaultRecordSet vrs;
humans.Request(mvlt::And(
        mvlt::Or(mvlt::Equal("Nationality", Nationality::Russia), mvlt::Equal("Nationality", Nationality::Britain)),
        mvlt::And(mvlt::Equal("Gender", true), (mvlt::Greater("Age", 35)))), 
    vrs);
```

Output the result of the request:

``` bash
+-----+--------+--------+-------------+
| Age | Gender | Name   | Nationality |
+-----+--------+--------+-------------+
| 65  | Male   | John   | Britain     |
| 46  | Male   | Artyom | Russia      |
+-----+--------+--------+-------------+
 (2 records)
```

Full programm code:
```cpp
#include "MVault.h"

// g++ main.cpp -IMVault/include -LMVault/bin -lMVault -lpthread

enum class Nationality
{
    Earth,
    Russia,
    China,
    Spain,
    Britain
};

template <>
std::string mvlt::ToString(const Nationality& nation) 
{
    switch (nation) 
    {
        case Nationality::Earth:
            return "Cosmopolitan";
        case Nationality::Russia:
            return "Russia";
        case Nationality::China:
            return "China";
        case Nationality::Spain:
            return "Spain";
        case Nationality::Britain:
            return "Britain";
    }
    return "";
}

int main()
{
    mvlt::Vault humans;

    humans.AddKey("Name", std::string(""));
    humans.AddKey("Gender", true);
    humans.AddKey("Age", -1);
    humans.AddKey("Nationality", Nationality::Earth);    

    humans.CreateRecord({ {"Name", std::string("Anton")}, {"Gender", true}, {"Age", 32}, {"Nationality", Nationality::Russia} });
    humans.CreateRecord({ {"Name", std::string("Artyom")}, {"Gender", true}, {"Age", 46}, {"Nationality", Nationality::Russia} });
    humans.CreateRecord({ {"Name", std::string("Oksana")}, {"Gender", false}, {"Age", 18}, {"Nationality", Nationality::Russia} });
    humans.CreateRecord({ {"Name", std::string("Muchen")}, {"Gender", true}, {"Age", 97}, {"Nationality", Nationality::China} });
    humans.CreateRecord({ {"Name", std::string("Yinuo")}, {"Gender", false}, {"Age", 21}, {"Nationality", Nationality::China} });
    humans.CreateRecord({ {"Name", std::string("Maria")}, {"Gender", false}, {"Age", 37}, {"Nationality", Nationality::Spain} });
    humans.CreateRecord({ {"Name", std::string("Huan")}, {"Gender", true}, {"Age", 48}, {"Nationality", Nationality::Spain} });
    humans.CreateRecord({ {"Name", std::string("John")}, {"Gender", true}, {"Age", 65}, {"Nationality", Nationality::Britain} });
    humans.CreateRecord({ {"Name", std::string("Diana")}, {"Gender", false}, {"Age", 26}, {"Nationality", Nationality::Britain} });
    humans.CreateRecord({ {"Name", std::string("Baro")}, {"Gender", true}, {"Age", 28}, {"Nationality", Nationality::Earth} });

    humans.PrintAsTable();

    mvlt::VaultRecordSet vrs;
    humans.Request(mvlt::And(
            mvlt::Or(mvlt::Equal("Nationality", Nationality::Russia), mvlt::Equal("Nationality", Nationality::Britain)),
            mvlt::And(mvlt::Equal("Gender", true), (mvlt::Greater("Age", 35)))), 
        vrs);
    vrs.PrintAsTable();
}
```