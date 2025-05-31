# Installation guide
The library is provided in two forms: release and debug.
The release was built with the *O2* optimization flag and does not support advanced debugging.
The debug release was built without optimization,
but it supports advanced debugging and will report errors.  
First of all, [download](https://github.com/mrognor/MVault/releases) a project release suitable for your environment.  
The header files are located in the *include* folder. 
The binary file of the library is located in the *lib* folder. 
The library is called *libMVault.a*.

## Build by hands
For manual assembly, you need to run the command:

```c++
g++ main.cpp -IMVault/include -LMVault/lib -lMVault
```

In this command, the keys mean the following:
* -IMVault/include - the folder with header files
* -LMVault/lib - the folder with the library's binary files
* -lMVault - library linking

To generate *compiler_commands.json*. You can use bear for syntax highlighting in the ide.

```c++
bear -- g++ main.cpp -IMVault/include -LMVault/lib -lMVault
```

After that, you need to restart the ide.

## Make
To build using a *Makefile*, use the following text:

```
demo: main.cpp
	g++ main.cpp -IMVault/include -LMVault/lib -lMVault -o demo

all: demo
```

## CMake
To build using *CMake*, use the following text:

```
project(demo)

add_executable(demo main.cpp)
include_directories(${CMAKE_SOURCE_DIR}/MVault/include)
target_link_libraries(demo ${CMAKE_SOURCE_DIR}/MVault/lib/libMVault.a)
```