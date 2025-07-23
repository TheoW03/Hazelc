#include <iostream>
#include <Demoduerlization.h>

DemodularizedVisitor::DemodularizedVisitor(IntermediateScope s)
{
    this->modules = s;
}

void DemodularizedVisitor::Visit(ProgramNode *node)
{

    // this->program.avail_modules = node->avail_modules;
    for (const auto &pair : node->avail_modules)
    {
        // this->current_AST_module = this->modules.modules[pair.first];
        modules.set_current_module(pair.first);
        pair.second->Accept(this);
    }
    std::cout << "hazelc: Demodularization" << std::endl;
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
        this->program.functions.push_back(node->functions[i]);

        node->functions[i]->Accept(this);
    }
}
void DemodularizedVisitor::Visit(BooleanExprNode *node)
{
    node->lhs->Accept(this);
    node->rhs->Accept(this);
}
void DemodularizedVisitor::Visit(ExprNode *node)
{
    node->lhs->Accept(this);
    node->rhs->Accept(this);
}
void DemodularizedVisitor::Visit(FunctionNode *node)
{
    node->hash_name = modules.get_global_function_hash(node->f->FunctionName);
    node->stmnts->Accept(this);
}
void DemodularizedVisitor::Visit(ConditionalNode *node)
{
    for (int i = 0; i < node->branches.size(); i++)
    {
        node->branches[i]->condition->Accept(this);
        node->branches[i]->stmnts->Accept(this);
    }
}
void DemodularizedVisitor::Visit(BlockNode *node)
{
    for (int i = 0; i < node->functions.size(); i++)
    {
        program.functions.push_back(node->functions[i]);
        node->functions[i]->Accept(this);
    }
    node->exit->Expr->Accept(this);
}
void DemodularizedVisitor::Visit(FunctionCallNode *node)
{

    for (int i = 0; i < node->param_types.size(); i++)
    {
        std::vector<std::shared_ptr<FunctionNode>> stmnts;
        auto ret = std::make_shared<ReturnNode>(node->params[i]);
        auto block = std::make_shared<BlockNode>(stmnts, ret);
        block->Accept(this);
        auto param_func = std::make_shared<FunctionNode>(false, true, true, node->param_types[i], block);
        program.functions.push_back(param_func);
    }
    node->hash_name = modules.get_global_function_hash(node->name);
    // std::cout << "function call: " << node->hash_name << std::endl;
}