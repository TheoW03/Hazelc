# Hazel

![](https://img.shields.io/badge/License-GPLv3-blue.svg)
![](https://tokei.rs/b1/github/TheoW03/hazel)

Hazel is a declarative functional paradigm programming language. 

This is the Compiler for it, hazelc

## build instrcutions

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

this is a fairly early project so most features are not impl yet 
please read the Langauge definition. 

[documentation]

[documentation]: <https://docs.google.com/document/d/12JWLuCte1r-tfAHKesndeBFRdnv7tT663QnIC2gSafY/edit?tab=t.0#heading=h.85eivsfcywf4>

[llvm]: <https://releases.llvm.org/download.html>

[cmake]: <https://cmake.org/download/>

[lines-of-code-badge]: <https://tokei.rs/b1/github/TheoW03/Hazel>
[gpl]:<https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0>