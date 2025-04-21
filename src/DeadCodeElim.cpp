#include <optimization/DeadCodeElim.h>

/*
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
bool DeadCode::isVisited(Tokens value)
{

    return this->global.find(value.value) != this->global.end() || this->local.find(value.value) != this->local.end();
}
void DeadCode::Visit(ProgramNode *node)
{
    auto main = node->getMainFunction().value();
    this->f.push_back(main);
    this->global.insert(main->f->FunctionName.value);
    main->Accept(this);
}

void DeadCode::Visit(FunctionNode *node)
{
    this->local.clear();
    node->stmnts[node->stmnts.size() - 1]->Accept(this);
    while (!this->func_calls.empty())
    {
        auto function_call_name = this->func_calls.front();
        this->func_calls.pop();
        if (!isVisited(function_call_name))
        {
                }
    }
}

void DeadCode::Visit(ExprNode *node)
{
    node->lhs->Accept(this);
    node->rhs->Accept(this);
}

void DeadCode::Visit(FunctionCallNode *node)
{
    this->func_calls.push(node->name);
}

void DeadCode::Visit(ReturnNode *node)
{
    node->Expr->Accept(this);
}
