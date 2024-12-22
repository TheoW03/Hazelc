
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <iostream>
#include "llvm/IR/LLVMContext.h"
void InitCompiler(std::string file_name)
{
    llvm::LLVMContext context;
    llvm::Module module("my_module", context);
    module.print(llvm::outs(), nullptr);
}