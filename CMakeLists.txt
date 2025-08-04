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
   src/InitHazel.cpp

   src/Util/Visitor.cpp
   src/Util/Error.cpp
   src/Util/CommandLineArguments.cpp
   
   src/Frontend/Lexxer.cpp
   src/Frontend/Parser.cpp
   src/Frontend/AstNodes.cpp

   src/IntermediatePasses/ConstantFolding.cpp
   src/IntermediatePasses/SemanticGlobalScope.cpp
   src/IntermediatePasses/TreeShake.cpp
   src/IntermediatePasses/SemanticLocalScope.cpp
   src/IntermediatePasses/DeadCodeElim.cpp
   src/IntermediatePasses/TypeChecker.cpp
   src/IntermediatePasses/DemodularizationPass.cpp
   src/IntermediatePasses/RecursiveImports.cpp
   src/IntermediatePasses/InliningFunctions.cpp

   
   src/LLVMBackend/Compiler.cpp
   src/LLVMBackend/CompileStatement.cpp
   src/LLVMBackend/CompileExpressions.cpp
   src/LLVMBackend/CompileHighLevel.cpp
   src/LLVMBackend/CompilerUtil.cpp
   src/LLVMBackend/CompiledTypes.cpp
   src/LLVMBackend/CompiledFunctions.cpp
   src/LLVMBackend/CompilerContext.cpp
   src/LLVMBackend/CRuntimeProcudures.cpp
   

)
target_include_directories(${EXECUTABLE_NAME} PRIVATE src/include/)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/..)
