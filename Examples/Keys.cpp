/*
    This file shows how to work with keys.
    It contains examples:
    1. Adding a key
    2. Adding a unique key
    3. Key update
    4. Deleting the key
*/

#include <MVault.h>

void AddingKey()
{
    std::cout << "Adding key" << std::endl;
    mvlt::Vault vlt;

    // Add key A with type int and default value 0
    vlt.AddKey("A", 0);

    // Static error! The string type is not explicitly specified
    // vlt.AddKey("B", "Hello world!");

    // Add key B with type string and empty default value
    vlt.AddKey<std::string>("B", "");

    vlt.Print();
}

void AddingUniqueKey()
{
    std::cout << "\n" << "Adding unique key" << std::endl;
    mvlt::Vault vlt;

    vlt.AddKey("A", 0);

    // The unique key type must be specified explicitly
    vlt.AddUniqueKey<std::size_t>("B");

    vlt.Print();

    vlt.CreateRecord({{"A", 1}, {"B", std::size_t(1)}});

    // Adding a unique key to a non-empty vault
    vlt.AddUniqueKey<std::size_t>("C", [](const std::size_t& counter, const mvlt::VaultRecordRef&) -> std::size_t
    {
        return 100 + counter;
    });

    vlt.Print();
}

void UpdatingKey()
{
    std::cout << "\n" << "Update key" << std::endl;
    mvlt::Vault vlt;

    vlt.AddKey("A", 0);

    vlt.CreateRecord({});

    // Update the default value for the key
    vlt.UpdateKey("A", 1);

    vlt.CreateRecord({});

    vlt.Print();
}

void RemovingKey()
{
    std::cout << "\n" << "Remove key" << std::endl;
    mvlt::Vault vlt;

    vlt.AddKey("A", 0);
    
    vlt.Print();

    // Remove key from vault
    vlt.RemoveKey("A");

    vlt.Print();
}

int main()
{
    AddingKey();
    AddingUniqueKey();
    UpdatingKey();
    RemovingKey();
}