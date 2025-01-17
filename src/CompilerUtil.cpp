#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <Frontend/Ast.h>
#include "llvm/IR/LLVMContext.h"
#include <memory>

llvm::Type *compileType(llvm::IRBuilder<> &builder, std::shared_ptr<Type> ty)
{
    if (dynamic_cast<NativeType *>(ty.get()))
    {
        auto p = dynamic_cast<NativeType *>(ty.get());
        if (p->type.type == TokenType::Integer || p->type.type == TokenType::Uinteger)
        {
            return builder.getInt64Ty();
        }
        else if (p->type.type == TokenType::Decimal)
        {
            return builder.getFloatTy();
        }
        else if (p->type.type == TokenType::Byte || p->type.type == TokenType::character || p->type.type == TokenType::Ubyte)
        {
            return builder.getInt8Ty();
        }
        else if (p->type.type == TokenType::boolean)
        {
            return builder.getInt1Ty();
        }
    }
    return builder.getVoidTy();
}

llvm::FunctionType *CompileFunctionType(llvm::IRBuilder<> &builder, std::shared_ptr<FunctionRefNode> n)
{
    auto c = n->RetType;
    std::vector<llvm::Type *> a;
    for (int i = 0; i < n->params.size(); i++)
    {
        auto funct = CompileFunctionType(builder, n->params[i]);
        a.push_back(llvm::PointerType::getUnqual(funct));
    }
    llvm::FunctionType *functype = llvm::FunctionType::get(
        compileType(builder, c), a, false);
    return functype;
}