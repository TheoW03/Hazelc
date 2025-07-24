cmake_minimum_required(VERSION 3.22.1)
project(Hazel)

find_package(LLVM 17 REQUIRED CONFIG)
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
if(DEFINED PROFILING AND PROFILING STREQUAL "ON")
   string(APPEND CMAKE_CXX_FLAGS " -pg")
   string(APPEND CMAKE_EXE_LINKER_FLAGS " -pg")
endif()

set(EXECUTABLE_NAME hazelc)

# Add your source files to an executable target

add_llvm_executable(${EXECUTABLE_NAME} 
   src/main.cpp
   src/Lexxer.cpp
   src/Parser.cpp
   src/AstNodes.cpp
   src/InitHazel.cpp
   src/Compiler.cpp
   src/ConstantFolding.cpp
   src/Visitor.cpp
   src/CompileStatement.cpp
   src/CompileExpressions.cpp
   src/CommandLineArguments.cpp
   src/CompileHighLevel.cpp
   src/CompilerUtil.cpp
   src/CompiledTypes.cpp
   src/CompiledFunctions.cpp
   src/CompilerContext.cpp
   src/SemanticGlobalScope.cpp
   src/TreeShake.cpp
   src/SemanticLocalScope.cpp
   src/DeadCodeElim.cpp
   src/TypeChecker.cpp
   src/CRuntimeProcudures.cpp
   src/Error.cpp
   src/DemodularizationPass.cpp

)
target_include_directories(${EXECUTABLE_NAME} PRIVATE src/include/)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/..)
