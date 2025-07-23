# Hazelc

![](https://img.shields.io/badge/License-GPLv3-blue.svg)
![](https://tokei.rs/b1/github/TheoW03/hazel)

Hazel is an open source, purely functional, declartive programming language that is designed 
to make functional programming more apporachable for people coming from imparaitive langauges such as C.


## Build Instructions

make sure you have CMake v3.22.1 and the LLVM version 17.0.0
if not download them here

- [cmake]
 
- [llvm]


now run this in your terminal

```sh
cmake -S . -B build 
cmake --build build --config Release -DPROFILING={ON|OFF}
./hazelc --help
```

## Documentation

- [langauge-definition]

- [examples]

# Progress

This a list of features I have implemented in the compiler

## Code Structures

- [x] Comments
- [x] Functions
- [x] Conditionals
- [x] Modules and Mdules Imports
- [ ] Lists
- [x] Function calls without parameters
- [ ] Function calls with parameters
- [ ] Anonmyous Functions

## Types

- [x] Native Types
- [ ] Struct Types
- [ ] Generics
- [ ] Variants (enum)
- [ ] Function Types
- [x] None

## Operarors

- [x] Basic Math Operators
- [x] Boolean operators
- [ ] String operators(concation done, Substring and charat needs some work)
- [x] Bitwise operators
- [ ] list operators
- [ ] unary operators



[llvm]: <https://releases.llvm.org/download.html>

[cmake]: <https://cmake.org/download/>
[langauge-definition]: <https://github.com/TheoW03/Hazelc/blob/master/Hazel_docs.pdf>
[examples]: <https://github.com/TheoW03/Hazelc/tree/master/samples>
[lines-of-code-badge]: <https://tokei.rs/b1/github/TheoW03/Hazel>
[gpl]:<https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0>