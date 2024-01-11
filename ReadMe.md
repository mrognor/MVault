Full documentation available [here](https://mrognor.github.io/DataStorage/index.html)

# Configure project
Cmake is used to configure the project. It is called via the **Configure** script. 
The **Configure** script can accept command-line parameters, which it will pass to the Cmake call.


# Old
This is readme

Full documentation available [here](https://mrognor.github.io/DataStorage/index.html)

This project provides a convenient way to work with data.

Be careful when adding a string as a key value. If you do not specialize the function type manually, 
then the compiler will perceive your string as an array of charms, which may cause errors when adding and reading data.
<span style="color:red">Add examples</span>.

Можно заменить или добавить схему на которой будет таблица
<img src="Docs/Scheme.svg" alt="scheme" width="100%"/>

Прописать про параметры cmake при вызове Configure.bat