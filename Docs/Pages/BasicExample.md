# Basic example

## Vault class
The main class of the entire library is Vault. This class is a table.
It supports the operations of adding, deleting and changing keys.  
It also supports record creation, deletion, and search operations.  

### Create Vault
The code for creating the table:

```c++
mvlt::Vault vlt;
```

### Add key to Vault
To add a key, a template method is used that accepts a string as the key and almost any type as the default key value.
The conditions for using the type as a value will be written later.

```c++
vlt.AddKey<std::string>("Name", "");
vlt.AddKey<std::string>("City", "");
vlt.AddKey<bool>("Gender", true);
vlt.AddKey<std::uint16_t>("Birth Year", 0);
```

Explicit type specification is not required, but it is recommended to explicitly specify the type, 
as this will help avoid errors when the type does not match.

### Create record
The createRecord method is used to create the record. 
It takes a vector with pairs of keys and values. 
The key is passed as a string, and a special class with a non-explicit constructor is used to pass the value.
This allows you to pass variables of different types through a single non-template interface.

```c++
vlt.CreateRecord({{"Name", std::string("Alexei Ivanov")}, {"City", std::string("Moscow")},
                  {"Gender", true}, {"Birth Year", std::uint16_t(1988)}});
```

### Remove record
You can delete records via VaultRecordRef or via the key and its value.

```c++
// VaultRecordRef vrr
vlt.EraseRecord(vrr);
vlt.EraseRecord<std::string>("Name", "Alexei Ivanov");
```

## VaultRecordRef class
The VaultRecordRef class is used to store record data. 
It is a lightweight wrapper over a pointer to a record. 
Through its methods, you can change the data inside the Vault and it will be fully synchronized with the changes inside the Vault.

### Find record
To search for a record, a template method is used that accepts a string with a key and a template value for searching, 
and then stores the value in VaultRecordRef.

```c++
mvlt::VaultRecordRef vrr;
vlt.GetRecord<std::string>("Name", "Alexei Ivanov", vrr);
```

### Update data
To change the record data, a template method is used that accepts a key and a new value.

```c++
vrr.SetData({{"City", std::string("Dubai")}});
```