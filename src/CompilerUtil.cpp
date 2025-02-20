#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <Frontend/Ast.h>
#include "llvm/IR/LLVMContext.h"
#include <memory>
#include <backend/CompilerUtil.h>

TypeOfExpr get_bool_expr_type(std::shared_ptr<ASTNode> n)
{
    auto c = dynamic_cast<BooleanExprNode *>(n.get());
    if (dynamic_cast<IntegerNode *>(c->lhs.get()) && dynamic_cast<IntegerNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    else if (dynamic_cast<DecimalNode *>(c->lhs.get()) && dynamic_cast<DecimalNode *>(c->rhs.get()))
        return TypeOfExpr::Float_Type;
    else if (dynamic_cast<BooleanConstNode *>(c->lhs.get()) && dynamic_cast<BooleanConstNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    if (dynamic_cast<BooleanExprNode *>(c->lhs.get()))
        return get_bool_expr_type(c->lhs);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return get_bool_expr_type(c->rhs);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return get_bool_expr_type(c->rhs);
}
TypeOfExpr get_expr_type(std::shared_ptr<ASTNode> n)
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
    if (dynamic_cast<ExprNode *>(c->lhs.get()))
        return get_expr_type(c->lhs);
    if (dynamic_cast<ExprNode *>(c->rhs.get()))
        return get_expr_type(c->rhs);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return get_bool_expr_type(c->rhs);
    if (dynamic_cast<BooleanExprNode *>(c->lhs.get()))
        return get_bool_expr_type(c->lhs);
}
std::shared_ptr<ASTNode> fold(std::shared_ptr<ASTNode> node)
{

    if (dynamic_cast<ExprNode *>(node.get()))
    {
        auto f = dynamic_cast<ExprNode *>(node.get());
        auto lhs = fold(f->lhs);
        auto rhs = fold(f->rhs);
        if (dynamic_cast<StringNode *>(lhs.get()) && dynamic_cast<StringNode *>(rhs.get()))
        {
            auto str_lhs = dynamic_cast<StringNode *>(lhs.get());
            auto str_rhs = dynamic_cast<StringNode *>(rhs.get());
            Tokens newStrToken = {str_lhs->value.value + str_rhs->value.value, TokenType::String_Lit, str_lhs->value.line_num};
            return std::make_shared<StringNode>(newStrToken);
        }
        // auto c = std::make_shared<ExprNode>(dynamic_cast<IntegerNode *>(node.get()));
        // return c;
    }
    return node;
}
OptionalType::OptionalType()
{
    /* compiler stop bitching */
}
OptionalType::OptionalType(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Type *inner)

{
    this->type = llvm::StructType::create(context, "OptionalType");
    std::vector<llvm::Type *> elements = {inner, builder.getInt1Ty()};
    this->type->setBody(elements);
}
