#include <optimization/InliningFunctions.h>

// we fill a hashmap of all functions that are considered constant
// meaning there return value is infact a constant
// if we fine such thing we put it in the hashmap we then check every return for its usage
// and substitute it with the return value if used

// example
// consider
// let foo : string => "foo"
// let bar : string => foo ++ "bar"
// becomes after inling pass
// let foo : string => "foo"
// let bar : string => "foo" ++ "bar"

bool check_ret(std::shared_ptr<ASTNode> node)
{
    return dynamic_cast<IntegerNode *>(node.get())    //
           || dynamic_cast<DecimalNode *>(node.get()) //
           || dynamic_cast<StringNode *>(node.get())  //
           || dynamic_cast<CharNode *>(node.get());   //
}

void InlineTopLevelVisitor::Visit(DemoduarlizedProgramNode *node)
{
    for (const auto &pair : node->global_functions)
    {
        auto function = pair.second;
        if (check_ret(function->stmnts->exit->Expr) && function->hash_name.has_value())
        {
            this->functions.insert(std::make_pair(function->hash_name.value(), function->stmnts->exit));
        }
    }
    for (int i = 0; i < node->functions.size(); i++)
    {
        node->functions[i]->Accept(this);
    }
}

void InlineTopLevelVisitor::Visit(FunctionNode *node)
{
    if (!node->hash_name.has_value() && check_ret(node->stmnts->exit->Expr))
    {
        if (this->functions.find(node->f->FunctionName.value) != this->functions.end())
        {
            this->functions[node->f->FunctionName.value] = node->stmnts->exit;
        }
        else
        {
            this->functions.insert(std::make_pair(node->f->FunctionName.value, node->stmnts->exit));
        }
    }
    for (int i = 0; i < node->stmnts->functions.size(); i++)
    {
        node->stmnts->functions[i]->Accept(this);
    }
    node->stmnts->exit->Expr = substitute(node->stmnts->exit->Expr);
}

std::shared_ptr<ASTNode> InlineTopLevelVisitor::substitute(std::shared_ptr<ASTNode> node)
{
    if (dynamic_cast<FunctionCallNode *>(node.get()))
    {
        auto c = dynamic_cast<FunctionCallNode *>(node.get());
        if (this->functions.find(c->hash_name.value_or(c->name.value)) != functions.end())
        {
            return this->functions[c->hash_name.value_or(c->name.value)]->Expr;
        }
    }
    if (dynamic_cast<ExprNode *>(node.get()))
    {
        auto c = dynamic_cast<ExprNode *>(node.get());
        c->lhs = substitute(c->lhs);
        c->rhs = substitute(c->rhs);
        return std::make_shared<ExprNode>(c->lhs, c->operation, c->rhs);
    }
    return node;
}
