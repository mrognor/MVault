# Requests

## VaultRecordSet class
VaultRecordSet is used to store data from Vault and is inherited from it. 
This class supports all the search functions from Vault and is fully synchronized with its parent Vault.

## Ready requests
There are ready-made methods with a single key and value query.

```c++
// mvlt::VaultRecordSet vrs;
vlt.RequestEqual("A", 7, vrs);
vlt.RequestGreater("A", 7, vrs);
vlt.RequestGreateOrEqual("A", 7, vrs);
vlt.RequestLess("A", 7, vrs);
vlt.RequestLessOrEqual("A", 7, vrs);
vlt.RequestInterval("A", 7, 9, vrs);
```

## Simple requests
Simple requests are made through a single method that accepts an object of the request class.

```c++
vlt.Request(mvlt::Equal("A", 7), vrs);
vlt.Request(mvlt::Greater("A", 7), vrs);
vlt.Request(mvlt::GreaterOrEqual("A", 7), vrs);
vlt.Request(mvlt::Less("A", 7), vrs);
vlt.Request(mvlt::LessOrEqual("A", 7), vrs);
```

## Complex requests
Complex requests allow you to write it using several different keys.

```c++
vlt.Request(mvlt::Less("A", 3) || mvlt::Greater("B", 7), vrs);
```

## Predicats
All request types support predicates. 
These are the functions that are responsible for adding a record to the VaultRecordSet when requested.
The function is called for each record that matches the request condition, 
and its return value is used to add the record.

```c++
vlt.Request(mvlt::Greater("A", 4, [](const mvlt::VaultRecordRef& vrr) -> bool
{
    int B = 0;
    vrr.GetData("B", B);
    if (B < 7)
        return true;
    else
        return false;
}), vrs);
```