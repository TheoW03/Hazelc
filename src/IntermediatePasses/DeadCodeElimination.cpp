#include <optimization/DeadCodeElim.h>

/*

    Very expiremental as of right now


    the LLVM does DCE for you. however I want control over it so im doint it myself.

    how it works is that you first visit the main function, add it to a list structure
    since Hazel has a property thats not in C. IE all functions must be pure functions. meaning tgehy mist return
    we can assume that the last absolute stament in a function is a return

    we visit from there. we check if it references a function call in its return. if it does
    we add it to a list and find the function.we visit from it.

    if a function is not in the function node we dont geerate it.


*/
DeadCode::DeadCode()
{
}

void DeadCode::Visit(DemoduarlizedProgramNode *node)
{
    auto main = node->getMainFunction().value();
    this->functions = node->global_functions;
    this->workList.push(main);
    while (!workList.empty())
    {
        auto f = workList.top();
        workList.pop();
        if (f->hash_name.has_value())
        {

            this->functions = node->global_functions;
        }
        if (!isVisited(f))
        {

            f->Accept(this);

            functions_referenced.push_back(f);
            this->visited.insert(f->hash_name.value_or(f->f->FunctionName.value));
        }
    }
    node->functions = functions_referenced;
}

bool DeadCode::isVisited(std::shared_ptr<FunctionNode> node)
{
    if (this->visited.find(node->hash_name.value_or(node->f->FunctionName.value)) != this->visited.end() //
        && this->functions.find(node->hash_name.value_or(node->f->FunctionName.value)) != this->functions.end())
    {
        return true;
    }
    return false;
}

void DeadCode::Visit(FunctionNode *node)
{
    node->stmnts->Accept(this);
}

void DeadCode::Visit(ExprNode *node)
{
    node->lhs->Accept(this);
    node->rhs->Accept(this);
}

void DeadCode::Visit(FunctionCallNode *node)
{
    this->workList.push(this->functions[node->hash_name.value_or(node->name.value)]);
}

void DeadCode::Visit(BlockNode *node)
{

    for (int i = 0; i < node->functions.size(); i++)
    {
        auto name = node->functions[i]->f->FunctionName;

        if (!node->functions[i]->hash_name.has_value())
        {
            if (this->functions.find(name.value) != this->functions.end())
            {
                this->functions[name.value] = node->functions[i];
            }
            else
            {
                this->functions.insert(std::make_pair(name.value, node->functions[i]));
            }
            this->visited.erase(name.value);
        }
    }
    node->exit->Accept(this);
}

void DeadCode::Visit(ReturnNode *node)
{
    node->Expr->Accept(this);
}
