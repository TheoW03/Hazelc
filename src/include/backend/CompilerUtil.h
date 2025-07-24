#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <map>
#include <iostream>
#include <queue>
#include <stack>
#include <Frontend/Ast.h>
// #include <backend/Compiled_Functions.h>
#include <Frontend/Token.h>
#include "llvm/IR/LLVMContext.h"

#ifndef PHI_NODE_STRUCTURE
#define PHI_NODE_STRUCTURE
struct ValueStruct
{
    llvm::BasicBlock *block;
    llvm::Value *value;
};
#endif

// #ifndef COMPILED_MODULE_H
// #define COMPILED_MODULE_H
// struct CompiledModule
// {
//     std::map<std::string, Function> func_map;
//     std::map<std::string, Function> export_function;
//     std::vector<Tokens> imports;
//     std::stack<Function> functions;
// };
// #endif

llvm::Value *ValueOrLoad(llvm::IRBuilder<> &builder, llvm::Value *value, llvm::Type *type);

#ifndef CFUNC_H
#define CFUNC_H

class CRunTimeFunctions
{
public:
    llvm::Function *memcpy;
    llvm::Function *malloc;
    llvm::Function *printf;
    llvm::Function *strcmp;
    CRunTimeFunctions();
    CRunTimeFunctions(llvm::IRBuilder<> &builder, llvm::Module &module);
};

#endif
