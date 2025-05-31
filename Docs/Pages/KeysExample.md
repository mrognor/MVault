# Keys example

## Adding key
The AddKey method is used to add the key. 
The values of this key are not unique in the Vault. 
It accepts a template parameter for the default key value. 
It is recommended to use an explicit.

Add key A with type int and default value 0.

```c++
vlt.AddKey("A", 0);
```

Static error! The string type is not explicitly specified.

```c++
vlt.AddKey("B", "Hello world!");
```

Add key B with type string and empty default value.

```c++
vlt.AddKey<std::string>("B", "");
```

## Adding unique key
Since the key is unique, it does not have a default value and it is required to explicitly specify the desired type.

```c++
vlt.AddUniqueKey<std::size_t>("B");
```

To add a unique key to a Vault that already has records, you need to use the lambda function. 
This lambda is called for every existing record in the database. 
It accepts a VaultRecordRef with a record and a counter with a record number. 
The function should return the value of the unique key.

```c++
vlt.AddUniqueKey<std::size_t>("C", [](const std::size_t& counter, const mvlt::VaultRecordRef&) -> std::size_t
{
    return 100 + counter;
});
```

## Update key
Update the default value for the key

```c++
vlt.UpdateKey("A", 1);
```

## Remove key
Remove key from vault

```c++
vlt.RemoveKey("A");
```