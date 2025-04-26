#include <optimization/ConstantFolding.h>
// this is a pass designed to fold constants
// it goes accroess to all expr Nodes and if they are constant expressions
// like "1+1" it solves it for 2.
// it substitues with solved values

// This is already in the LLVM. however since all types in Hazel are optional structures
// we cant rely on the LLVM for accurate constant folding. or atleast at -O0.
ConstantFoldingVisitor::ConstantFoldingVisitor()
{
}
void ConstantFoldingVisitor::Visit(ASTNode *node)
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
    node->Expr->Accept(this);
}

void ConstantFoldingVisitor::Visit(FunctionCallNode *node)
{
    for (int i = 0; i < node->params.size(); i++)
    {
        FoldExpr expr;
        node->params[i] = expr.fold_expr(node->params[i]);
    }
}

void ConstantFoldingVisitor::Visit(ExprNode *node)
{
    node->lhs->Accept(this);
    node->rhs->Accept(this);
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

    switch (op.type)
    {
    case Addition:
        return std::make_shared<IntegerNode>(lhs->number + rhs->number);
        break;
    case Subtraction:
        return std::make_shared<IntegerNode>(lhs->number - rhs->number);
        break;
    case Multiplication:
        return std::make_shared<IntegerNode>(lhs->number * rhs->number);
        break;
    case Division:
        return std::make_shared<IntegerNode>(lhs->number / rhs->number);
        break;
    case Modulas:
        return std::make_shared<IntegerNode>(lhs->number % rhs->number);
        break;
    case And:
        return std::make_shared<IntegerNode>(lhs->number & rhs->number);
        break;
    case Or:
        return std::make_shared<IntegerNode>(lhs->number | rhs->number);
        break;
    case Left_Shift:
    {
        return std::make_shared<IntegerNode>(lhs->number << rhs->number);
        break;
    }

    case Right_Shift:
        return std::make_shared<IntegerNode>(lhs->number >> rhs->number);
        break;
    default:
        break;
    }
    return nullptr;
}

std::shared_ptr<ASTNode> FoldExpr::fold_decimal(std::shared_ptr<DecimalNode> lhs, Tokens op, std::shared_ptr<DecimalNode> rhs)
{
    switch (op.type)
    {
    case Addition:
        return std::make_shared<DecimalNode>(lhs->number + rhs->number);
        break;
    case Subtraction:
        return std::make_shared<DecimalNode>(lhs->number - rhs->number);
        break;
    case Multiplication:
        return std::make_shared<DecimalNode>(lhs->number * rhs->number);
        break;
    case Division:
        return std::make_shared<DecimalNode>(lhs->number / rhs->number);
        break;
    }
    return nullptr;
}

std::shared_ptr<ASTNode> FoldExpr::fold_string(std::shared_ptr<StringNode> lhs, Tokens op, std::shared_ptr<StringNode> rhs)
{
    switch (op.type)
    {
    case Concation:
    {
        return std::make_shared<StringNode>(lhs->value + rhs->value);
        break;
    }
    }
    return nullptr;
}

std::shared_ptr<ASTNode> FoldExpr::fold_bool_integer(std::shared_ptr<IntegerNode> lhs, Tokens op, std::shared_ptr<IntegerNode> rhs)
{
    switch (op.type)
    {
    case EQ:
        return std::make_shared<BooleanConstNode>(lhs->number == rhs->number);
    case NE:
        return std::make_shared<BooleanConstNode>(lhs->number != rhs->number);
    case LT:
        return std::make_shared<BooleanConstNode>(lhs->number < rhs->number);
    case LTE:
        return std::make_shared<BooleanConstNode>(lhs->number <= rhs->number);
    case GT:
        return std::make_shared<BooleanConstNode>(lhs->number > rhs->number);
    case GTE:
        return std::make_shared<BooleanConstNode>(lhs->number >= rhs->number);
    }
    return nullptr;
}

std::shared_ptr<ASTNode> FoldExpr::fold_bool_const(std::shared_ptr<BooleanConstNode> lhs, Tokens op, std::shared_ptr<BooleanConstNode> rhs)
{
    switch (op.type)
    {
    case And:
        return std::make_shared<BooleanConstNode>(lhs->val && rhs->val);
    case Or:
        return std::make_shared<BooleanConstNode>(lhs->val || rhs->val);
    }
    return nullptr;
}

std::shared_ptr<ASTNode> FoldExpr::fold_bool_bool(std::shared_ptr<BooleanConstNode> lhs, Tokens op, std::shared_ptr<BooleanConstNode> rhs)
{
    switch (op.type)
    {
    case EQ:
        return std::make_shared<BooleanConstNode>(lhs->val == rhs->val);
    case NE:
        return std::make_shared<BooleanConstNode>(lhs->val != rhs->val);
    case LT:
        return std::make_shared<BooleanConstNode>(lhs->val < rhs->val);
    case LTE:
        return std::make_shared<BooleanConstNode>(lhs->val <= rhs->val);
    case GT:
        return std::make_shared<BooleanConstNode>(lhs->val > rhs->val);
    case GTE:
        return std::make_shared<BooleanConstNode>(lhs->val >= rhs->val);
    }
    return nullptr;
}

std::shared_ptr<ASTNode> FoldExpr::fold_bool_decimal(std::shared_ptr<DecimalNode> lhs, Tokens op, std::shared_ptr<DecimalNode> rhs)
{
    switch (op.type)
    {
    case EQ:
        return std::make_shared<BooleanConstNode>(lhs->number == rhs->number);
    case NE:
        return std::make_shared<BooleanConstNode>(lhs->number != rhs->number);
    case LT:
        return std::make_shared<BooleanConstNode>(lhs->number < rhs->number);
    case LTE:
        return std::make_shared<BooleanConstNode>(lhs->number <= rhs->number);
    case GT:
        return std::make_shared<BooleanConstNode>(lhs->number > rhs->number);
    case GTE:
        return std::make_shared<BooleanConstNode>(lhs->number >= rhs->number);
    }
    return nullptr;
}

std::shared_ptr<ASTNode> FoldExpr::fold_bool_string(std::shared_ptr<StringNode> lhs, Tokens op, std::shared_ptr<StringNode> rhs)
{
    switch (op.type)
    {
    case EQ:
        return std::make_shared<BooleanConstNode>(lhs->value == rhs->value);
    case NE:
        return std::make_shared<BooleanConstNode>(lhs->value != rhs->value);
    default:
        break;
    }
    return std::shared_ptr<ASTNode>();
}
std::shared_ptr<ASTNode> FoldExpr::fold_expr(std::shared_ptr<ASTNode> n)
{

    if (dynamic_cast<ExprNode *>(n.get()))
    {
        auto expr = dynamic_cast<ExprNode *>(n.get());
        auto lhs = fold_expr(expr->lhs);
        auto rhs = fold_expr(expr->rhs);

        // if they are constants
        // we solve them and create a new node with the solved expression
        if (dynamic_cast<IntegerNode *>(lhs.get()) && dynamic_cast<IntegerNode *>(rhs.get()))
        {
            auto lhsInt = dynamic_cast<IntegerNode *>(lhs.get())->number;
            auto rhsInt = dynamic_cast<IntegerNode *>(rhs.get())->number;

            return fold_integer(std::make_shared<IntegerNode>(lhsInt),
                                expr->operation, std::make_shared<IntegerNode>(rhsInt));
        }
        else if (dynamic_cast<DecimalNode *>(lhs.get()) && dynamic_cast<DecimalNode *>(rhs.get()))
        {
            auto lhsInt = dynamic_cast<DecimalNode *>(lhs.get())->number;
            auto rhsInt = dynamic_cast<DecimalNode *>(rhs.get())->number;
            return fold_decimal(std::make_shared<DecimalNode>(lhsInt),
                                expr->operation, std::make_shared<DecimalNode>(rhsInt));
        }
        else if (dynamic_cast<StringNode *>(lhs.get()) && dynamic_cast<StringNode *>(rhs.get()))
        {
            auto lhsInt = dynamic_cast<StringNode *>(lhs.get())->value;
            auto rhsInt = dynamic_cast<StringNode *>(rhs.get())->value;
            return fold_string(std::make_shared<StringNode>(lhsInt),
                               expr->operation, std::make_shared<StringNode>(rhsInt));
        }
        else if (dynamic_cast<BooleanConstNode *>(lhs.get()) && dynamic_cast<BooleanConstNode *>(rhs.get()))
        {
            auto lhsInt = dynamic_cast<BooleanConstNode *>(lhs.get())->val;
            auto rhsInt = dynamic_cast<BooleanConstNode *>(rhs.get())->val;
            return fold_bool_const(std::make_shared<BooleanConstNode>(lhsInt),
                                   expr->operation, std::make_shared<BooleanConstNode>(rhsInt));
        }
        return std::make_shared<ExprNode>(lhs, expr->operation, rhs);
    }
    else if (dynamic_cast<BooleanExprNode *>(n.get()))
    {
        auto expr = dynamic_cast<BooleanExprNode *>(n.get());
        auto lhs = fold_expr(expr->lhs);
        auto rhs = fold_expr(expr->rhs);
        if (dynamic_cast<IntegerNode *>(lhs.get()) && dynamic_cast<IntegerNode *>(rhs.get()))
        {
            auto lhsInt = dynamic_cast<IntegerNode *>(lhs.get())->number;
            auto rhsInt = dynamic_cast<IntegerNode *>(rhs.get())->number;

            return fold_bool_integer(std::make_shared<IntegerNode>(lhsInt),
                                     expr->op, std::make_shared<IntegerNode>(rhsInt));
        }
        if (dynamic_cast<StringNode *>(lhs.get()) && dynamic_cast<StringNode *>(rhs.get()))
        {
            auto lhsInt = dynamic_cast<StringNode *>(lhs.get())->value;
            auto rhsInt = dynamic_cast<StringNode *>(rhs.get())->value;
            return fold_bool_string(std::make_shared<StringNode>(lhsInt),
                                    expr->op, std::make_shared<StringNode>(rhsInt));
        }
        if (dynamic_cast<DecimalNode *>(lhs.get()) && dynamic_cast<DecimalNode *>(rhs.get()))
        {
            auto lhsInt = dynamic_cast<DecimalNode *>(lhs.get())->number;
            auto rhsInt = dynamic_cast<DecimalNode *>(rhs.get())->number;

            return fold_bool_decimal(std::make_shared<DecimalNode>(lhsInt),
                                     expr->op, std::make_shared<DecimalNode>(rhsInt));
        }
        if (dynamic_cast<BooleanConstNode *>(lhs.get()) && dynamic_cast<BooleanConstNode *>(rhs.get()))
        {
            auto lhsInt = dynamic_cast<BooleanConstNode *>(lhs.get())->val;
            auto rhsInt = dynamic_cast<BooleanConstNode *>(rhs.get())->val;

            return fold_bool_integer(std::make_shared<IntegerNode>(lhsInt),
                                     expr->op, std::make_shared<IntegerNode>(rhsInt));
        }
        return std::make_shared<BooleanExprNode>(lhs, expr->op, rhs);
    }
    return n;
}
