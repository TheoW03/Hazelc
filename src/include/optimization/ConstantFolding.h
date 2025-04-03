#include <visitor.h>

#ifndef FOLD_EXPR_H
#define FOLD_EXPR_H
class FoldExpr
{
private:
    std::shared_ptr<ASTNode> fold_integer(std::shared_ptr<IntegerNode> lhs, Tokens op, std::shared_ptr<IntegerNode> rhs);
    std::shared_ptr<ASTNode> fold_decimal(std::shared_ptr<DecimalNode> lhs, Tokens op, std::shared_ptr<DecimalNode> rhs);
    std::shared_ptr<ASTNode> fold_string(std::shared_ptr<StringNode> lhs, Tokens op, std::shared_ptr<StringNode> rhs);

public:
    FoldExpr();
    std::shared_ptr<ASTNode> fold_expr(std::shared_ptr<ASTNode> n);
};

#endif

#ifndef SEMANTIC_H
#define SEMANTIC_H
class ConstantFoldingVisitor : public Visitor
{
public:
    ConstantFoldingVisitor();
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ModuleNode *node) override;
    void Visit(ReturnNode *node) override;
    void Visit(FunctionCallNode *node) override;
    void Visit(ProgramNode *node) override;
};
#endif
