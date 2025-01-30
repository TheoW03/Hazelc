#include <backend/CompilerUtil.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
Function CompilerContext::get_function(Tokens name)
{
    return func_map[name.value];
}
void CompilerContext::add_function(Tokens name, Function f)
{
    this->func_map.insert(std::make_pair(name.value, f));
}
llvm::StructType *CompilerContext::get_string_type(llvm::LLVMContext &context, llvm::IRBuilder<> &builder)
{
    if (this->string_type == nullptr)
    {

        this->string_type = llvm::StructType::create(context, "string");
        std::vector<llvm::Type *> elements = {builder.getInt8PtrTy(), builder.getInt64Ty()};
        this->string_type->setBody(elements);
    }
    return string_type;
    // this->string_type =
    // return llvm::StructType();
}