#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <Frontend/Ast.h>
#include "llvm/IR/LLVMContext.h"
#include <memory>
#include <backend/CompilerContext.h>
#include "llvm/IR/Intrinsics.h"

// TypeOfExpr get_type_func_call(std::shared_ptr<Compiled_Function> function)
// {
//     auto f = function->get_ret_type();
//     if (dynamic_cast<IntegerType *>(f.get()) || dynamic_cast<ByteType *>(f.get()))
//     {
//         return TypeOfExpr::Integer_Type;
//     }
//     else if (dynamic_cast<DecimalType *>(f.get()))
//     {
//         return TypeOfExpr::Float_Type;
//     }
//     else if (dynamic_cast<BoolType *>(f.get()))
//     {
//         return TypeOfExpr::Boolean_Type;
//     }
//     else if (dynamic_cast<StringType *>(f.get()))
//     {
//         return TypeOfExpr::String_Type;
//     }
//     return TypeOfExpr::Void_Type;
// }

// // these get the type of the expression
// // used because the LLCM has seperation for floats and
// TypeOfExpr
// get_binary_bool_expr_type(std::shared_ptr<ASTNode> n, CompilerContext ctx)
// {
//     TypeOfExpr get_binary_expr_type(std::shared_ptr<ASTNode> n, CompilerContext ctx);
//     auto c = dynamic_cast<BooleanExprNode *>(n.get());
//     if (dynamic_cast<IntegerNode *>(c->lhs.get()) && dynamic_cast<IntegerNode *>(c->rhs.get()))
//         return TypeOfExpr::Integer_Type;
//     else if (dynamic_cast<DecimalNode *>(c->lhs.get()) && dynamic_cast<DecimalNode *>(c->rhs.get()))
//         return TypeOfExpr::Float_Type;
//     else if (dynamic_cast<BooleanConstNode *>(c->lhs.get()) && dynamic_cast<BooleanConstNode *>(c->rhs.get()))
//         return TypeOfExpr::Boolean_Type;
//     else if (dynamic_cast<StringNode *>(c->lhs.get()) && dynamic_cast<StringNode *>(c->rhs.get()))
//         return TypeOfExpr::String_Type;

//     else if (dynamic_cast<NoneNode *>(c->lhs.get()) || dynamic_cast<NoneNode *>(c->rhs.get()))
//         return TypeOfExpr::None_Type;

//     if (dynamic_cast<FunctionCallNode *>(c->lhs.get()))
//     {
//         auto func = dynamic_cast<FunctionCallNode *>(c->lhs.get());
//         auto f = ctx.get_function(func).value();
//         return get_type_func_call(f);
//     }
//     if (dynamic_cast<FunctionCallNode *>(c->rhs.get()))
//     {
//         auto func = dynamic_cast<FunctionCallNode *>(c->rhs.get());
//         auto f = ctx.get_function(func).value();
//         return get_type_func_call(f);
//     }

//     if (dynamic_cast<BooleanExprNode *>(c->lhs.get()))
//         return get_binary_bool_expr_type(c->lhs, ctx);
//     if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
//         return get_binary_bool_expr_type(c->rhs, ctx);
//     if (dynamic_cast<ExprNode *>(c->rhs.get()))
//         return get_binary_expr_type(c->rhs, ctx);
//     if (dynamic_cast<ExprNode *>(c->lhs.get()))
//         return get_binary_expr_type(c->lhs, ctx);
//     return TypeOfExpr::Void_Type;
// }
// TypeOfExpr get_binary_expr_type(std::shared_ptr<ASTNode> n, CompilerContext ctx)
// {
//     auto c = dynamic_cast<ExprNode *>(n.get());
//     if (dynamic_cast<IntegerNode *>(c->lhs.get()) && dynamic_cast<IntegerNode *>(c->rhs.get()))
//         return TypeOfExpr::Integer_Type;
//     else if (dynamic_cast<DecimalNode *>(c->lhs.get()) && dynamic_cast<DecimalNode *>(c->rhs.get()))
//         return TypeOfExpr::Float_Type;
//     else if (dynamic_cast<BooleanConstNode *>(c->lhs.get()) && dynamic_cast<BooleanConstNode *>(c->rhs.get()))
//         return TypeOfExpr::Integer_Type;
//     else if (dynamic_cast<StringNode *>(c->lhs.get()) && dynamic_cast<StringNode *>(c->rhs.get()))
//         return TypeOfExpr::String_Type;
//     if (dynamic_cast<FunctionCallNode *>(c->lhs.get()))
//     {
//         auto func = dynamic_cast<FunctionCallNode *>(c->lhs.get());
//         auto f = ctx.get_function(func).value();
//         return get_type_func_call(f);
//     }
//     if (dynamic_cast<FunctionCallNode *>(c->rhs.get()))
//     {
//         auto func = dynamic_cast<FunctionCallNode *>(c->rhs.get());
//         auto f = ctx.get_function(func).value();
//         return get_type_func_call(f);
//     }

//     if (dynamic_cast<ExprNode *>(c->lhs.get()))
//         return get_binary_expr_type(c->lhs, ctx);
//     if (dynamic_cast<ExprNode *>(c->rhs.get()))
//         return get_binary_expr_type(c->rhs, ctx);
//     if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
//         return TypeOfExpr::Boolean_Type;
//     if (dynamic_cast<BooleanExprNode *>(c->lhs.get()))
//         return TypeOfExpr::Boolean_Type;
//     return TypeOfExpr::Void_Type;
// }

// std::optional<OptionalType> get_type_unary(std::shared_ptr<ASTNode> n, CompilerContext ctx)
// {
//     if (dynamic_cast<IntegerNode *>(n.get()))
//         return ctx.get_integer_type();
//     if (dynamic_cast<DecimalNode *>(n.get()))
//         return ctx.get_float_type();
//     if (dynamic_cast<StringNode *>(n.get()))
//         return ctx.get_string_type();
//     if (dynamic_cast<BooleanExprNode *>(n.get()))
//     {
//         return ctx.get_boolean_type();
//     }
//     if (dynamic_cast<ExprNode *>(n.get()))
//     {
//         auto a = dynamic_cast<ExprNode *>(n.get());
//         return get_type_unary(a->rhs, ctx);
//     }
//     if (dynamic_cast<FunctionCallNode *>(n.get()))
//     {
//         auto d = dynamic_cast<FunctionCallNode *>(n.get());
//         // auto f = ctx.get_function(d->hash_name.has_value() ? d->hash_name.value() : d->name.value).value();
//         auto f = ctx.get_function(d).value()->get_ret_type();

//         if (dynamic_cast<IntegerType *>(f.get()) || dynamic_cast<ByteType *>(f.get()))
//         {
//             return ctx.get_integer_type();
//         }
//         else if (dynamic_cast<DecimalType *>(f.get()))
//         {
//             return ctx.get_float_type();
//         }
//         else if (dynamic_cast<BoolType *>(f.get()))
//         {
//             return ctx.get_boolean_type();
//         }
//         else if (dynamic_cast<StringType *>(f.get()))
//         {
//             return ctx.get_string_type();
//         }
//     }
//     return {};
// }

llvm::Value *ValueOrLoad(llvm::IRBuilder<> &builder, llvm::Value *value, llvm::Type *type)
{
    if (value->getType()->isPointerTy())
        value = builder.CreateLoad(type, value, "ValueOrLoad");
    return value;
}
