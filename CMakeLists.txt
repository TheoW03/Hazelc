cmake_minimum_required(VERSION 3.22.1)
project(SimpleProject)

find_package(LLVM REQUIRED CONFIG)
set(CMAKE_CXX_STANDARD 23)  # Requires C++20

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.
include_directories(${LLVM_INCLUDE_DIRS})
# include_directories()

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Now build our tools
# Specify the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/..)
message("Include paths of YourDependency: ${YourDependency_INCLUDE_DIRS}")

if(DEFINED DEBUG AND DEBUG)
   message("test")
endif()
include_directories(${EXECUTABLE_NAME} PRIVATE src/include/)

# Name of your output executable
set(EXECUTABLE_NAME psuedoc)

# Add your source files to an executable target
add_executable(${EXECUTABLE_NAME} 
   src/main.cpp
   src/lexxer.cpp
   src/parser.cpp
   src/Ast.cpp


)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/include)

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
target_link_libraries(${EXECUTABLE_NAME} ${llvm_libs})
