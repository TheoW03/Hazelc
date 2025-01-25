#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <Frontend/Ast.h>
#include "llvm/IR/LLVMContext.h"
#ifndef TYPE_OF_EXPR_H
#define TYPE_OF_EXPR_H
enum TypeOfExpr
{
    Boolean_Type,
    Integer_Type,
    Float_Type
};
#endif
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

TypeOfExpr get_expr_type(std::shared_ptr<ASTNode> n);
TypeOfExpr get_bool_expr_type(std::shared_ptr<ASTNode> n);