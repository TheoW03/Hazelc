# Hazelc

![](https://img.shields.io/badge/License-GPLv3-blue.svg)
![](https://tokei.rs/b1/github/TheoW03/hazel)

Hazel is an open source, purely functional, declartive programming language that is designed 
to make functional programming more apporachable for people coming from imparaitive langauges such as C.

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



[llvm]: <https://releases.llvm.org/download.html>

[cmake]: <https://cmake.org/download/>

[lines-of-code-badge]: <https://tokei.rs/b1/github/TheoW03/Hazel>
[gpl]:<https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0>