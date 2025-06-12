#include <iostream>
#include <Demoduerlization.h>

DemodularizedVisitor::DemodularizedVisitor(IntermediateScope s)
{
    this->modules = s;
}

void DemodularizedVisitor::Visit(ProgramNode *node)
{

    this->program.avail_modules = node->avail_modules;
    for (const auto &pair : node->avail_modules)
    {
        // this->current_AST_module = this->modules.modules[pair.first];
        modules.set_current_module(pair.first);
        pair.second->Accept(this);
    }
}

void DemodularizedVisitor::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->functions.size(); i++)
    {
        this->program.global_functions.insert(
            std::make_pair(modules.get_global_function_hash(
                                      node->functions[i]->f->FunctionName)
                               .value(),
                           node->functions[i]));
        node->functions[i]->Accept(this);
    }
}
void DemodularizedVisitor::Visit(FunctionNode *node)
{
    // std::cout << node->f->FunctionName.value << " hash: " << modules.get_global_function_hash(node->f->FunctionName).value() << std::endl;
    node->hash_name = modules.get_global_function_hash(node->f->FunctionName).value();
    node->stmnts->exit->Expr->Accept(this);
}

void DemodularizedVisitor::Visit(FunctionCallNode *node)
{
    node->hash_name = modules.get_global_function_hash(node->name).value_or("");
    // std::cout << "function call: " << node->hash_name << std::endl;
}