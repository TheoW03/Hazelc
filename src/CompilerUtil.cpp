#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <Frontend/Ast.h>
#include "llvm/IR/LLVMContext.h"
#include <memory>
#include <backend/CompilerContext.h>

TypeOfExpr get_bool_expr_type(std::shared_ptr<ASTNode> n, ProgramScope ctx)
{
    auto c = dynamic_cast<BooleanExprNode *>(n.get());
    if (dynamic_cast<IntegerNode *>(c->lhs.get()) && dynamic_cast<IntegerNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    else if (dynamic_cast<DecimalNode *>(c->lhs.get()) && dynamic_cast<DecimalNode *>(c->rhs.get()))
        return TypeOfExpr::Float_Type;
    else if (dynamic_cast<BooleanConstNode *>(c->lhs.get()) && dynamic_cast<BooleanConstNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    if (dynamic_cast<BooleanExprNode *>(c->lhs.get()))
        return get_bool_expr_type(c->lhs, ctx);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return get_bool_expr_type(c->rhs, ctx);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return get_bool_expr_type(c->rhs, ctx);
    return TypeOfExpr::Void_Type;
}
TypeOfExpr get_expr_type(std::shared_ptr<ASTNode> n, ProgramScope ctx)
{
    auto c = dynamic_cast<ExprNode *>(n.get());
    if (dynamic_cast<IntegerNode *>(c->lhs.get()) && dynamic_cast<IntegerNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    else if (dynamic_cast<DecimalNode *>(c->lhs.get()) && dynamic_cast<DecimalNode *>(c->rhs.get()))
        return TypeOfExpr::Float_Type;
    else if (dynamic_cast<BooleanConstNode *>(c->lhs.get()) && dynamic_cast<BooleanConstNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    else if (dynamic_cast<StringNode *>(c->lhs.get()) && dynamic_cast<StringNode *>(c->rhs.get()))
        return TypeOfExpr::String_Type;
    else if (dynamic_cast<FunctionCallNode *>(c->lhs.get()))
    {
        auto d = dynamic_cast<FunctionCallNode *>(c->lhs.get());
        auto f = ctx.get_function(d->name);
        if (dynamic_cast<NativeType *>(f.ret_type.get()))
        {
            auto p = dynamic_cast<NativeType *>(f.ret_type.get());
            if (p->type.type == TokenType::Integer)
            {
                return TypeOfExpr::Integer_Type;
            }
            else if (p->type.type == TokenType::Integer)
            {
                return TypeOfExpr::Float_Type;
            }
            else if (p->type.type == TokenType::Integer)
            {
                return TypeOfExpr::String_Type;
            }
        }
        return TypeOfExpr::Void_Type;
        // if (f.)
        // auto c =
    }

    if (dynamic_cast<ExprNode *>(c->lhs.get()))
        return get_expr_type(c->lhs, ctx);
    if (dynamic_cast<ExprNode *>(c->rhs.get()))
        return get_expr_type(c->rhs, ctx);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return get_bool_expr_type(c->rhs, ctx);
    if (dynamic_cast<BooleanExprNode *>(c->lhs.get()))
        return get_bool_expr_type(c->lhs, ctx);
    return TypeOfExpr::Void_Type;
}
// std::shared_ptr<ASTNode> fold(std::shared_ptr<ASTNode> node)
// {

//     if (dynamic_cast<ExprNode *>(node.get()))
//     {
//         auto f = dynamic_cast<ExprNode *>(node.get());
//         auto lhs = fold(f->lhs);
//         auto rhs = fold(f->rhs);
//         if (dynamic_cast<StringNode *>(lhs.get()) && dynamic_cast<StringNode *>(rhs.get()))
//         {
//             auto str_lhs = dynamic_cast<StringNode *>(lhs.get());
//             auto str_rhs = dynamic_cast<StringNode *>(rhs.get());
//             Tokens newStrToken = {str_lhs->value.value + str_rhs->value.value, TokenType::String_Lit, str_lhs->value.line_num};
//             return std::make_shared<StringNode>(newStrToken);
//         }
//         // auto c = std::make_shared<ExprNode>(dynamic_cast<IntegerNode *>(node.get()));
//         // return c;
//     }
//     return node;
// }
OptionalType::OptionalType()
{
    /* compiler stop bitching */
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
    auto destField0ptr = builder.CreateStructGEP(this->type, structPtr, 0, "OptionalStructPtr0");

    builder.CreateStore(value, destField0ptr);
    auto destField1ptr = builder.CreateStructGEP(this->type, structPtr, 1, "OptionalStructPtr1");
    auto isNone = llvm::ConstantInt::get(builder.getInt1Ty(), 0);
    builder.CreateStore(isNone, destField1ptr);
    // structPtr->dump();
    return structPtr;
}

llvm::Value *OptionalType::get_value(llvm::IRBuilder<> &builder)
{
    return nullptr;
}

llvm::Value *OptionalType::get_none(llvm::IRBuilder<> &builder)
{
    llvm::Value *structPtr = builder.CreateAlloca(this->type);
    auto destField1ptr = builder.CreateStructGEP(this->type, structPtr, 1, "OptionalStructPtr1");
    auto isNone = llvm::ConstantInt::get(builder.getInt1Ty(), 1);
    builder.CreateStore(isNone, destField1ptr);

    return structPtr;
}

llvm::Type *OptionalType::get_type()
{
    return this->type;
}

llvm::Value *ValueOrLoad(llvm::IRBuilder<> &builder, llvm::Value *value, llvm::Type *type)
{
    if (value->getType()->isPointerTy())
        value = builder.CreateLoad(type, value);
    return value;
}