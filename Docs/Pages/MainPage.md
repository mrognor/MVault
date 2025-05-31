\mainpage

A simple in-memory table database focused on maximum performance and reliability.
Provided as a dependless library with a simple and beautiful API.

# Key features
1. The STL strings are used as keys.
2. Type safety.
3. A custom data type can be used inside the database.
4. Supports complex queries and subqueries.
5. Full multithreaded code support.
6. Using efficient data structures such as hash tables and binary trees to store data.
7. CSV support.
8. Exceptless code.

# Quick start

First of all, download the debug library [binaries](https://github.com/mrognor/MVault/releases).
After that, write a simple program.

```c++
#include <MVault.h>

int main()
{
    mvlt::Vault vlt;
    vlt.AddKey("A", 0);
    vlt.CreateRecord({});
    vlt.Print();
}
```

Now compile the example using the command:

```bash
g++ main.cpp -IMVaultD/include -LMVaultD/lib -lMVault
```

Call a.out and get the output:

```
+---+
| A |
+---+
| 0 |
+---+
 (1 records)
```

# Guides
See the instructions for installing the library [here](InstallationGuide.md).  
Check the examples of using the library:
1. Basic [example](BasicExample.md)
2. Keys [example](KeysExample.md)
3. Requests [example](RequestsExample.md)

# FAQ
* **How do I display the contents of my class?**  
To do this, define the ToString function for your type. More information is available [here](group___to_string.html).
* **How to use custom class inside vault?**  
To do this, you need to define a hashing function from std for your data type. More information is available [here](CustomTypeFAQ.md).