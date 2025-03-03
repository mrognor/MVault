#include <MVault.h>

int main()
{
    /**
        \page SortingExample
        
        # An example of working with sorted data
        
        Vault allows you to work with sorted data. It is important to understand that inside the Vault 
        there are binary trees in which keys are stored, so all data is already sorted by all keys. 
        This page will demonstrate how to work with sorted data.
        
        There are two ways to work with sorted data:
        1. Getting a vector with records
        2. Record processing via function transfer

        ## Preparation
        First, let's create a Vault. Let's say it will be filled with users. 
        Each user will have 2 parameters. Let's add 3 keys to the Vault, 
        one service key to uniquely identify each record and two to represent the user. 
        1. int id - the unique identifier of the record inside the Vault
        2. std::string name - the name of the user record
        3. bool gender - the user's gender
    */

    // startcode{.cpp}
    mvlt::Vault vlt;
    
    vlt.AddKey("id", -1);
    vlt.AddKey<std::string>("name", "");
    vlt.AddKey<bool>("gender", true);
    // endcode

    /**
        \page SortingExample

        Let's fill the Vault with 10 records
    */

    // startcode{.cpp}
    vlt.CreateRecord({ {"id", 1}, {"name", std::string("Marfa")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 2}, {"name", std::string("Alexander")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 3}, {"name", std::string("John")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 4}, {"name", std::string("Patricia")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 5}, {"name", std::string("Mikhail")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 6}, {"name", std::string("Daria")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 7}, {"name", std::string("Natasha")}, {"gender", false} });
    vlt.CreateRecord({ {"id", 8}, {"name", std::string("Jayson")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 9}, {"name", std::string("Alexander")}, {"gender", true} });
    vlt.CreateRecord({ {"id", 10}, {"name", std::string("Varvara")}, {"gender", false} });
    // endcode

    /**
        \page SortingExample

        Let's print Vault:
    */

    // startcode{.cpp}
    std::cout << "Vault: " << std::endl;
    vlt.Print();
    // endcode

    /**
        \page SortingExample
        
        As a result, the Vault will look like this:
        
        ```
        +--------+----+-----------+
        | gender | id | name      |
        +--------+----+-----------+
        | true   | 9  | Alexander |
        | true   | 8  | Jayson    |
        | false  | 7  | Natasha   |
        | false  | 6  | Daria     |
        | false  | 4  | Patricia  |
        | true   | 5  | Mikhail   |
        | true   | 3  | John      |
        | true   | 2  | Alexander |
        | false  | 10 | Varvara   |
        | false  | 1  | Marfa     |
        +--------+----+-----------+
        (10 records)
        ```

        ## Vector of records
        The first and easiest way to work with sorted data is to query the ref vector. 
        The refs in the vector are arranged in the order in which the sorted key values are located.

        ### First example

        Getting a vector with three records. The vector will contain 3 records with the smallest ids.
        At the beginning of the vector there will be an record with the smallest id
    */

    std::cout << "The first 3 records after sorting in ascending order by id: " << std::endl;
    // startcode{.cpp}
    std::vector<mvlt::VaultRecordRef> sorted = vlt.GetSortedRecords("id", false, 3);
    mvlt::PrintAsTable(sorted);
    // endcode

    /**
        \page SortingExample
        
        As a result, the vector will look like this:
        ```
        +--------+----+-----------+
        | gender | id | name      |
        +--------+----+-----------+
        | false  | 1  | Marfa     |
        | true   | 2  | Alexander |
        | true   | 3  | John      |
        +--------+----+-----------+
        (3 records)
        ```

        ### Second example
        In this example, 5 records are requested, sorted in reverse order.
        The array will be printed without the gender column.
    */

    std::cout << "The first 5 records without gender after sorting in descending order by name:" << std::endl;
    // startcode{.cpp}
    sorted = vlt.GetSortedRecords("name", true, 5);
    mvlt::PrintAsTable(sorted, false, -1, {"id", "name"});
    // endcode

    /**
        \page SortingExample
        
        As a result, the vector will look like this:
        ```
        +----+----------+
        | id | name     |
        +----+----------+
        | 10 | Varvara  |
        | 4  | Patricia |   
        | 7  | Natasha  |
        | 5  | Mikhail  |
        | 1  | Marfa    |
        +----+----------+
        (5 records)
        ```

        ## The function in the iteration
        In this method, you need to define a function. This function will be called for all records in sorted order.
        Using the return value, you can stop iterating through records. To do this, the function must return false

        ### Example

        In this example, we are looking for an record with the name Alexander and the largest id
    */

    std::cout << "The largest id with name Alexander: ";
    // startcode{.cpp}
    int greatestAlexanderId = -1;
    vlt.SortBy("id", [&](const mvlt::VaultRecordRef& recordRef)
        {
            std::string name;
            recordRef.GetData("name", name);
            if (name == "Alexander")
            {
                recordRef.GetData("id", greatestAlexanderId);
                return false;
            }
            else
                return true;
        }, true);
    std::cout << greatestAlexanderId << std::endl;
    // endcode

    
    /**
        \page SortingExample
        
        As a result, the function will print the following:

        ```bash
        The record with the largest id with name Alexander: 9
        ```

        Let's check it out. We will request all records with the name Alexander and print them.
    */

    std::cout << "Alexanders: " << std::endl;
    // startcode{.cpp}
    std::vector<mvlt::VaultRecordRef> alexanders;
    vlt.GetRecords("name", std::string("Alexander"), alexanders);
    mvlt::PrintAsTable(alexanders);
    // endcode

    /**
        \page SortingExample
        
        As a result, the function will print the following:

        ```bash
        +--------+----+-----------+
        | gender | id | name      |
        +--------+----+-----------+
        | true   | 2  | Alexander |
        | true   | 9  | Alexander |
        +--------+----+-----------+
        (2 records)
        ```

        As you can easily see, the largest id of an record with the name Alexander is 9.
    */
}