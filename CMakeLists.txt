cmake_minimum_required(VERSION 3.22.1)
project(SimpleProject)

find_package(LLVM REQUIRED CONFIG)
set(CMAKE_CXX_STANDARD 20)  # Requires C++20
include(AddLLVM)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions")
# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.
include_directories(${LLVM_INCLUDE_DIRS})
# include_directories()
# Name of your output executable

if(DEFINED DEBUG AND DEBUG)
   message("test")
endif()
set(EXECUTABLE_NAME hazelc)

# Add your source files to an executable target
include_directories(${EXECUTABLE_NAME} PRIVATE src/include/)

add_llvm_executable(${EXECUTABLE_NAME} 
   src/main.cpp
   src/lexxer.cpp
   src/parser.cpp
   src/Ast.cpp
   src/Compiler.cpp
   src/semantic_analysis.cpp
   src/visitor.cpp
   src/CompilerUtil.cpp

)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/..)
