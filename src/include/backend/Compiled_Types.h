
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#ifndef OPTIONAL_TYPE_H
#define OPTIONAL_TYPE_H
class OptionalType
{
public:
    llvm::StructType *type;
    llvm::Type *inner;
    OptionalType();
    OptionalType(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Type *inner);
    llvm::Value *set_loaded_value(llvm::Value *value, llvm::IRBuilder<> &builder);
    llvm::Value *set_loaded_struct_value(llvm::Function *memcpy, llvm::Value *value, llvm::IRBuilder<> &builder, size_t size);
    llvm::Value *get_value(llvm::IRBuilder<> &builder);
    llvm::Value *get_none(llvm::IRBuilder<> &builder);
    llvm::Type *get_type();

    llvm::Value *get_inner_value(llvm::IRBuilder<> &builder, llvm::Value *value, bool load);
    size_t get_type_size(llvm::Module &module);
    size_t get_inner_size(llvm::Module &module);
};
#endif
