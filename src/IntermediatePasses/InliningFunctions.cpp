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
    auto functions = node->functions;
    for (int i = 0; i < functions.size(); i++)
    {
        auto function = functions[i];
        if (check_ret(function->stmnts->exit->Expr) && function->hash_name.has_value())
        {
            this->functions.insert(std::make_pair(function->hash_name.value(), function->stmnts->exit));
        }
    }
    for (int i = 0; i < functions.size(); i++)
    {
        auto function = functions[i];
        function->Accept(this);
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
}

std::shared_ptr<ASTNode> InlineTopLevelVisitor::substitute(std::shared_ptr<ASTNode> node)
{
}
