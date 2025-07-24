
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <Frontend/Ast.h>
#include "llvm/IR/LLVMContext.h"
#include <memory>
#include <backend/Compiled_Types.h>
#include "llvm/IR/Intrinsics.h"

// optional type. since in hazel all types are optional
// it compies as a structure
//  the actual valye
// and a Boolean to represent if its none or some
OptionalType::OptionalType()
{
}
OptionalType::OptionalType(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Type *inner)

{
    this->type = llvm::StructType::create(context, "OptionalType");
    this->inner = inner;
    std::vector<llvm::Type *> elements = {inner, builder.getInt1Ty()};
    this->type->setBody(elements);
}

llvm::Value *OptionalType::set_loaded_value(llvm::Value *value, llvm::IRBuilder<> &builder)
{

    llvm::Value *structPtr = builder.CreateAlloca(this->type);
    auto destField0ptr = builder.CreateStructGEP(this->type, structPtr, 0, "OptionalStructPtr0.Value");

    builder.CreateStore(value, destField0ptr);
    auto destField1ptr = builder.CreateStructGEP(this->type, structPtr, 1, "OptionalStructPtr1.isNone");
    auto isNone = llvm::ConstantInt::get(builder.getInt1Ty(), 0);
    builder.CreateStore(isNone, destField1ptr);
    return structPtr;
}

llvm::Value *OptionalType::set_loaded_struct_value(llvm::Function *memcpy, llvm::Value *value, llvm::IRBuilder<> &builder, size_t size)
{
    llvm::Value *structPtr = builder.CreateAlloca(this->type);
    auto destField0ptr = builder.CreateStructGEP(this->type, structPtr, 0, "OptionalStructPtr0.Value");
    builder.CreateCall(memcpy, {
                                   destField0ptr,
                                   value,
                                   llvm::ConstantInt::get(builder.getInt64Ty(), size),
                                   llvm::ConstantInt::get(builder.getInt1Ty(), 0),

                               });
    auto isNone = llvm::ConstantInt::get(builder.getInt1Ty(), 0);
    auto destField1ptr = builder.CreateStructGEP(this->type, structPtr, 1, "OptionalStructPtr1.isNone");
    builder.CreateStore(isNone, destField1ptr);
    return structPtr;
}

llvm::Value *OptionalType::get_value(llvm::IRBuilder<> &builder)
{
    return nullptr;
}

llvm::Value *OptionalType::get_none(llvm::IRBuilder<> &builder)
{
    llvm::Value *structPtr = builder.CreateAlloca(this->type);
    auto destField0ptr = builder.CreateStructGEP(this->type, structPtr, 0, "OptionalStructPtr0.Value");

    builder.CreateStore(llvm::Constant::getNullValue(inner), destField0ptr);
    auto destField1ptr = builder.CreateStructGEP(this->type, structPtr, 1, "OptionalStructPtr1.isNone");
    auto isNone = llvm::ConstantInt::get(builder.getInt1Ty(), 1);
    builder.CreateStore(isNone, destField1ptr);
    return structPtr;
}

llvm::Type *OptionalType::get_type()
{
    return this->type;
}
llvm::Value *OptionalType::get_inner_value(llvm::IRBuilder<> &builder, llvm::Value *value, bool load)
{
    auto v = builder.CreateStructGEP(this->type, value, 0, "getting_inner_value");
    if (load)
    {
        return builder.CreateLoad(inner, v, "get inner and load");
    }
    return v;
}

size_t OptionalType::get_type_size(llvm::Module &module)
{
    llvm::DataLayout datalayout(&module);
    return datalayout.getTypeAllocSize(this->type);
}

size_t OptionalType::get_inner_size(llvm::Module &module)
{
    llvm::DataLayout datalayout(&module);

    return datalayout.getTypeAllocSize(this->inner);
}