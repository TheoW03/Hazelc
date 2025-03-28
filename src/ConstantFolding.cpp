#include <visitor.h>

void ConstantFoldingVisitor::Visit(ASTNode *node)
{
}
ConstantFoldingVisitor::ConstantFoldingVisitor()
{
}
void ConstantFoldingVisitor::Visit(FunctionNode *node)
{
    for (int i = 0; i < node->stmnts.size(); i++)
    {
        node->stmnts[i]->Accept(this);
    }
}
void ConstantFoldingVisitor::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->functions.size(); i++)
    {
        node->functions[i]->Accept(this);
    }
}
void ConstantFoldingVisitor::Visit(ReturnNode *node)
{
    FoldExpr expr;
    node->Expr = expr.fold_expr(node->Expr);
}

void ConstantFoldingVisitor::Visit(FunctionCallNode *node)
{
}

void ConstantFoldingVisitor::Visit(ProgramNode *node)
{
    for (const auto &[key, current_module] : node->avail_modules)
    {
        current_module->Accept(this);
    }
}

FoldExpr::FoldExpr()
{
}
std::shared_ptr<ASTNode> FoldExpr::fold_integer(std::shared_ptr<IntegerNode> lhs, Tokens op, std::shared_ptr<IntegerNode> rhs)
{
    std::cout << "number: " << lhs->number << std::endl;

    std::cout << "nuymber: " << rhs->number << std::endl;

    switch (op.type)
    {
    case Addition:
        return std::make_shared<IntegerNode>(lhs->number + rhs->number);
        break;
    default:
        break;
    }
    return nullptr;
}

std::shared_ptr<ASTNode> FoldExpr::fold_expr(std::shared_ptr<ASTNode> n)
{

    if (dynamic_cast<ExprNode *>(n.get()))
    {
        auto expr = dynamic_cast<ExprNode *>(n.get());
        auto lhs = fold_expr(expr->lhs);
        auto rhs = fold_expr(expr->rhs);
        if (dynamic_cast<IntegerNode *>(lhs.get()) && dynamic_cast<IntegerNode *>(rhs.get()))
        {
            auto lhsInt = dynamic_cast<IntegerNode *>(lhs.get())->number;
            auto rhsInt = dynamic_cast<IntegerNode *>(rhs.get())->number;

            return fold_integer(std::make_shared<IntegerNode>(lhsInt),
                                expr->operation, std::make_shared<IntegerNode>(rhsInt));
        }
        return std::make_shared<ExprNode>(lhs, expr->operation, rhs);
    }
    return n;
}
