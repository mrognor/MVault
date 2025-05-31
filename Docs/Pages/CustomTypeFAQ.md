# Custom Type
To use a custom class, it must support the following options:
1. An std::hash must be defined for it.
2. The operator less than must be defined.
3. The comparison operator must be defined.
4. Optionally, the ToString function should be defined to simplify debugging.

You can see how this is done in the examples in 
*[CustomType.cpp](https://github.com/mrognor/MVault/blob/master/Examples/CustomType.cpp)*