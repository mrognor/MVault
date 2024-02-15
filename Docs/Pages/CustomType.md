# Custom type storage inside the vault
Since Vault is built on the unordered_multimap from stl, in order to store data inside std::unordered_multimap, 
a hashing function from stl must be defined for this data type.  
To use the type inside the Vault, you need to specialize the ```struct std::hash<T>``` structure for your type.  
Inside this function, you need to define an operator() that accepts your type ```std::size_t operator()(const T& dat) const```.

## Example for std::vector<int>
Let's define the structure for the class of the vector of integers:

```cpp
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
```

## Example for custom class
Let's say you need to save the human class in the Vault. There are 3 fields in this class:
* Age - integer
* Name - string
* Gender - bool

The class will look like this:

```cpp
class Human
{
private:
    int Age = -1;
    std::string Name = "";
    bool Gender = true;

    friend std::hash<Human>;
};
```

Then the caching functionality will look like this:

```cpp
template <>
struct std::hash<Human>
{
    std::size_t operator()(const Human& human) const
    {
        return std::hash<int>()(human.Age) + std::hash<std::string>()(human.Name) + std::hash<bool>()(human.Gender);
    }
};
```