# MVault

<a href="https://github.com/mrognor/MVault/actions/workflows/tests.yml"><img src="https://github.com/mrognor/MVault/actions/workflows/tests.yml/badge.svg"/></a>
<a href="https://github.com/mrognor/MVault/actions/workflows/sast.yml"><img src="https://github.com/mrognor/MVault/actions/workflows/sast.yml/badge.svg"/></a>
<a href="https://github.com/mrognor/MVault/actions/workflows/github-code-scanning/codeql" alt="CodeQL"><img src="https://github.com/mrognor/MVault/actions/workflows/github-code-scanning/codeql/badge.svg" alt="CodeQL"/></a>
<a href="https://mrognor.github.io/MVault/"><img src="https://github.com/mrognor/MVault/actions/workflows/static.yml/badge.svg"/></a>
<br>
<img src="https://img.shields.io/badge/Solution-C++11-blue.svg?style=flat&logo=c%2B%2B&logoColor=b0c0c0&labelColor=363D44" alt="C++ solution"/>
<img src="https://img.shields.io/badge/OS-linux%20%7C%20windows%20%7C%20android-blue??style=flat&logo=Linux&logoColor=b0c0c0&labelColor=363D44" alt="Operating systems"/>
<img src="https://img.shields.io/badge/CPU-x86__64%20%7C%20aarch64%20%7C%20risc_v%20%7C%20e2k-blue?style=flat&logo=amd&logoColor=b0c0c0&labelColor=363D44" alt="CPU Architect"/>

A simple in-memory table database focused on maximum performance and reliability. 
Provided as a dependless library with a simple and beautiful API.

![Screencast_20250522_203536-ezgif com-video-to-gif-converter](https://github.com/user-attachments/assets/7198c57f-6c5e-416a-98c1-de2eb6c31088)

<details>
  <summary>Code and data from video:</summary>
<table class="no-border">
  <tr>
    <td>
      <pre><code>
// Create vlt
Vault vlt;


// Add keys
vlt.AddUniqueKey<string>("Letter");
vlt.AddKey("Frequency", 0);
vlt.AddKey("Percentage", 0.0);

// Read data
vlt.ReadFile("letters.csv");

// Show data
vlt.Print(false, 6);

// Get T letter record
VaultRecordRef vrr;
vlt.GetRecord<std::string>("Letter", "C", vrr);

// Show record
vrr.PrintRecord();

// Add new key and change data
vlt.AddKey<bool>("Handled", false);
vrr.SetData("Handled", true);

// Show data after updation
vlt.Print(false, 6);

// Request all records with percentage greater then 3
VaultRecordSet vrs;
vlt.Request(Greater("Percentage", 3.0), vrs);

// Show request result
vrs.Print(false, 6);
      </code></pre>
    </td>
    <td>
    <pre>
    Letter,Frequency,Percentage
A,24373121,8.1
B,4762938,1.6
C,8982417,3.0
D,10805580,3.6
E,37907119,12.6
F,7486889,2.5
G,5143059,1.7
H,18058207,6.0
I,21820970,7.3
J,474021,0.2
K,1720909,0.6
L,11730498,3.9
M,7391366,2.5
N,21402466,7.1
O,23215532,7.7
P,5719422,1.9
Q,297237,0.1
R,17897352,5.9
S,19059775,6.3
T,28691274,9.5
U,8022379,2.7
V,2835696,0.9
W,6505294,2.2
X,562732,0.2
Y,5910495,2.0
Z,93172,0.0
    </pre>
    </td>
  </tr>
</table>
</details>

## Table Of Contents

- [Project overview](#project-overview)
    - [Key features](#key-features)
    - [Code quality](#code-quality)
        - [Tests](#tests)
        - [Code coverage](#code-coverage)
        - [Sast](#sast)
        - [Documentation](#documentation)
- [Installation guide](#installation-guide)
    - [Make](#make)
    - [CMake](#cmake)
- [Simple demo](#simple-demo)
    - [Adding keys](#adding-keys)
    - [Creating records](#creating-records)
    - [Requests](#requests)
- [Compatibility](#compatibility)

## Project overview

### Key features
1. The STL strings are used as keys.
2. Type safety.
3. A custom data type can be used inside the database.
4. Supports complex queries and subqueries.
5. Full multithreaded code support.
6. Using efficient data structures such as hash tables and binary trees to store data.
7. CSV support.
8. Exceptless code.

### Code quality

#### Tests
The CI process on Github Actions is set up for the project. Unit tests are run on each commit to keep the project in working order.
At the moment, more than 500 tests have been written. 

![image](https://github.com/user-attachments/assets/91a008c0-f248-48f1-973e-3ffea2114d3b)

#### Code coverage
A test coverage verification process is set up for the project.
Currently the percentage of code coverage exceeds 95%. Test coverage data is created for each commit and is available on
[Github Actions](https://github.com/mrognor/MVault/actions/workflows/tests.yml).

![image](https://github.com/user-attachments/assets/0ff53344-d756-43c5-a81e-9f31992001f5)

#### SAST
The CI has a static code analysis process set up. After each commit, an analysis is performed using clangsa and clang_tidy. 
The results of the analysis are completely [open](https://github.com/mrognor/MVault/actions/workflows/sast.yml).

![image](https://github.com/user-attachments/assets/234dba95-d2ad-4e6c-ae8e-7f778a5b6174)

#### Documentation
Doxygen is used for documentation. Documentation is generated for each commit and hosted on [Github Pages](https://mrognor.github.io/MVault/).

# Installation guide
First of all, [download](https://github.com/mrognor/MVault/releases) a project release suitable for your environment.

## Make
Assuming you are in the working directory, the folder with the library is in the same directory.
Create a simple main.cpp file with minimal code:

```c++
#include <MVault.h>

int main()
{
    mvlt::Vault vlt;
    vlt.AddKey<std::string>("Key", "Hello world!");
    vlt.CreateRecord({});
    vlt.Print();
}
```

Now create a Makefile with this content:

```Makefile
demo: main.cpp
	g++ main.cpp -IMVault/include -LMVault/lib -lMVault -o demo

all: demo
```
Call `make` to build project.
Now check that the installation is correct by calling demo

![image](https://github.com/user-attachments/assets/032d761f-d7a7-48b0-9f4e-a1a910e9ddc3)

In order for syntax highlighting to work in your ide, you can use the bear utility.
```bash
bear -- make
```
Restart your ide so that the backlight appears.

## CMake
Assume that you are in the working directory, the folder with the library is in the same directory.
Create a simple main.cpp file with minimal code:

```c++
#include <MVault.h>

int main()
{
    mvlt::Vault vlt;
    vlt.AddKey<std::string>("Key", "Hello world!");
    vlt.CreateRecord({});
    vlt.Print();
}
```

Now create a CMakeLists.txt with this content:

```CMakeLists
cmake_minimum_required(VERSION 4.0)

project(demo)

add_executable(demo main.cpp)
include_directories(${CMAKE_SOURCE_DIR}/MVault/include)
target_link_libraries(demo ${CMAKE_SOURCE_DIR}/MVault/lib/libMVault.a)
```

Now build and run the project for verification

```bash
mkdir build
cd build
cmake ..
make
./demo
```

To create a database with compilation commands for your ide, use the CMAKE_EXPORT_COMPILE_COMMAND option when calling cmake.
```bash
cmake .. -D CMAKE_EXPORT_COMPILE_COMMANDS=1
```
# Simple demo
Let's write a simple program that stores information about airplane flights.
The initial code:
```c++
#include <MVault.h>

int main()
{
}
```
All subsequent code will be added to the main.

## Adding keys

Let the flight have the following set of parameters: unique id, flight number, departure airport, arrival airport.

```c++
// Create vault object
mvlt::Vault vlt;

// Add keys to vault
vlt.AddUniqueKey<std::size_t>("Id");
vlt.AddKey<int>("Flight", 0);
vlt.AddKey<std::string>("Arrival", "");
vlt.AddKey<std::string>("Departure", "");
```

## Creating records

Create 5 records

```c++
// Create records
vlt.CreateRecord({{"Id", 1ul}, {"Flight", 246}, {"Arrival", std::string("SVO")}, {"Departure", std::string("LED")}});
vlt.CreateRecord({{"Id", 2ul}, {"Flight", 117}, {"Arrival", std::string("DXB")}, {"Departure", std::string("SIN")}});
vlt.CreateRecord({{"Id", 3ul}, {"Flight", 203}, {"Arrival", std::string("SIP")}, {"Departure", std::string("SVO ")}});
vlt.CreateRecord({{"Id", 4ul}, {"Flight", 318}, {"Arrival", std::string("PEC")}, {"Departure", std::string("SHA")}});
vlt.CreateRecord({{"Id", 5ul}, {"Flight", 509}, {"Arrival", std::string("ADD")}, {"Departure", std::string("JNB")}});

// Print table
vlt.Print();
```

After compiling and running the program, the output will be as follows:

![image](https://github.com/user-attachments/assets/e80302f4-b61a-474f-9999-26bb2c32e64d)

## Requests

Now a request for all flights that are associated with SVO.

```c++
// Create set to store request result
mvlt::VaultRecordSet vrs;

// Request records with arrival or depature equals svo
vlt.Request(mvlt::Equal("Arrival", std::string("SVO")) || mvlt::Equal("Departure", std::string("SVO")), vrs);

// Print request result
vrs.Print();
```

The result will be like this:

![image](https://github.com/user-attachments/assets/2e470528-8de3-4f60-90ff-cc23646b9027)

# Compatibility

The library is written in c++11 without using third-party libraries, so it is built for almost any platform.

<table>
    <tr>
        <th>Os</th>
        <th>Arch</th>
        <th>Compiler</th>
        <th>Stdlib</th>
    </tr>
    <tr>
        <td rowspan="12">Linux</td>
        <td rowspan="4">amd64</td>
        <td rowspan="2">gcc</td>
        <td>glibc</td>
    </tr>
    <tr>
        <td>musl</td>
    </tr>
    <tr>
        <td rowspan="2">clang</td>
        <td>glibc</td>
    </tr>
    <tr>
        <td>musl</td>
    </tr>
    <tr>
        <td rowspan="3">aarch64</td>
        <td rowspan="2">gcc</td>
        <td>glibc</td>
    </tr>
    <tr>
        <td>musl</td>
    </tr>
    <tr>
        <td>clang</td>
        <td>glibc</td>
    </tr>
    <tr>
        <td>e2k</td>
        <td>lcc</td>
        <td>glibc</td>
    </tr>
    <tr>
        <td rowspan="4">risc-v</td>
        <td rowspan="2">gcc</td>
        <td>glibc</td>
    </tr>
    <tr>
        <td>musl</td>
    </tr>
    <tr>
        <td rowspan="2">clang</td>
        <td>glibc</td>
    </tr>
    <tr>
        <td>musl</td>
    </tr>
    <tr>
        <td>Android(termux)</td>
        <td>aarch64</td>
        <td>clang</td>
        <td>musl</td>
    </tr>
    <tr>
        <td rowspan="3">Windows</td>
        <td rowspan="3">amd64</td>
        <td>mingw(gcc)</td>
        <td>MSVCRT</td>
    </tr>
    <tr>
        <td>clang</td>
        <td>MSVCRT</td>
    </tr>
    <tr>
        <td>MSVC</td>
        <td>MSVCRT</td>
    </tr>
</table>
