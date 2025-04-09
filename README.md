# Hazelc

![](https://img.shields.io/badge/License-GPLv3-blue.svg)
![](https://tokei.rs/b1/github/TheoW03/hazel)

this a llvm backend compiler for the Hazel programming language.

## What is Hazel

Hazel is a purely functional, declartive programming language.

The syntax and design was heavily inspired by other functional languages like Haskell.


## Build Instructions

make sure you have CMake v3.22.1 and the LLVM version 14.0.0
if not download them here

- [cmake]
 
- [llvm]


now run this in your terminal

```sh
cmake -S . -B build 
cmake --build build --config Release -DPROFILING={ON|OFF}
./hazelc --help
```

> [!WARNING]  
> Hazelc is in the indev pre alpga phase.  
>
> if you want to consider using the Hazel programming langauge. Please reachout 



[llvm]: <https://releases.llvm.org/download.html>

[cmake]: <https://cmake.org/download/>

[lines-of-code-badge]: <https://tokei.rs/b1/github/TheoW03/Hazel>
[gpl]:<https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0>