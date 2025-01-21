#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <Frontend/Ast.h>
#include "llvm/IR/LLVMContext.h"

#ifndef COMPILED_FUNCTION_H
#define COMPILED_FUNCTION_H
struct Function
{
    llvm::Function *function;
    std::vector<Function> params;
};
#endif
llvm::Type *compileType(llvm::IRBuilder<> &builder, std::shared_ptr<Type> ty);
llvm::FunctionType *CompileFunctionType(llvm::IRBuilder<> &builder, std::shared_ptr<FunctionRefNode> n);