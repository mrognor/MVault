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

## Table Of Content

- [Project overview](#project-overview)
    - [Key features](#key-features)
    - [Code quality](#code-quality)
        - [Tests](#tests)
        - [Code coverage](#code-coverage)
        - [Sast](#sast)
        - [Documentation](#documentation)
  
## Project overview

### Key features
1. The stl strings are used as keys.
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
Now the percentage of code coverage exceeds 95%. Test coverage data is created for each commit and is available on
[Github Actions](https://github.com/mrognor/MVault/actions/workflows/tests.yml).

![image](https://github.com/user-attachments/assets/0ff53344-d756-43c5-a81e-9f31992001f5)

#### Sast
The CI has a static code analysis process set up. After each commit, an analysis is performed using clangsa and clang_tidy. 
The results of the analysis are completely [open](https://github.com/mrognor/MVault/actions/workflows/sast.yml).

![image](https://github.com/user-attachments/assets/234dba95-d2ad-4e6c-ae8e-7f778a5b6174)

#### Documentation
Doxygen is used for documentation. Documentation is generated for each commit and hosted on [Github Pages](https://mrognor.github.io/MVault/).
