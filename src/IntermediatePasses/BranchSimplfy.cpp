#include <optimization/BranchSimplfy.h>

/*
This pass goes through and removes conditionals that are 100% true
and any branch in the conditional thats 100% false

example:

let main : integer =>
conditional : integer
    true => 1
    $defualt => 2
becomes

let main : integer => 1

and if we have false

let main : integer =>
conditional : integer
    false => 1
    $defualt => 2
becomes

let main : integer =>
conditional : integer
    $defualt => 2


*/
BranchSimplfyVisitor::BranchSimplfyVisitor() {}
void BranchSimplfyVisitor::Visit(DemoduarlizedProgramNode *node)
{
    for (int i = 0; i < node->functions.size(); i++)
    {
        node->functions[i]->stmnts->Accept(this);
    }
    std::cout << "hazelc: simplfy branching" << std::endl;
}
void BranchSimplfyVisitor::Visit(FunctionNode *node)
{
}

void BranchSimplfyVisitor::Visit(BlockNode *node)
{
    node->exit->Expr = substitute(node->exit->Expr);
}

std::shared_ptr<ASTNode> BranchSimplfyVisitor::substitute(std::shared_ptr<ASTNode> node)
{

    if (dynamic_cast<ConditionalNode *>(node.get()))
    {

        auto c = dynamic_cast<ConditionalNode *>(node.get());
        if (c->branches.size() == 1)
        {
            return c->branches[0]->stmnts->exit->Expr;
        }
        for (int i = 0; i < c->branches.size() - 1; i++)
        {
            if (dynamic_cast<BooleanConstNode *>(c->branches[i]->condition.get()))
            {

                auto boolc = dynamic_cast<BooleanConstNode *>(c->branches[i]->condition.get());
                c->branches[i]->stmnts->Accept(this);
                if (boolc->val)
                {
                    return c->branches[i]->stmnts->exit->Expr;
                }
                if (!boolc->val)
                {
                    c->branches.erase(c->branches.begin() + i);
                }
            }
        }
        return node;
    }
    if (dynamic_cast<ExprNode *>(node.get()))
    {
        auto c = dynamic_cast<ExprNode *>(node.get());
        c->lhs = substitute(c->lhs);
        c->rhs = substitute(c->rhs);
        return std::make_shared<ExprNode>(c->lhs, c->operation, c->rhs);
    }
    if (dynamic_cast<BooleanExprNode *>(node.get()))
    {
        auto c = dynamic_cast<BooleanExprNode *>(node.get());
        c->lhs = substitute(c->lhs);
        c->rhs = substitute(c->rhs);
        return std::make_shared<BooleanExprNode>(c->lhs, c->op, c->rhs);
    }
    return node;
}
