#include <visitor.h>

SemanticLocalScopeVisitor::SemanticLocalScopeVisitor(std::map<std::string, SemanticModule> modules)
{
    this->modules = modules;
}

std::optional<int> SemanticLocalScopeVisitor::find_function_global(Tokens name)
{
    auto global_functions = this->current_AST_module.functions;
    if (global_functions.find(name.value) != global_functions.end())
    {
        return 1;
    }
    else
    {
        auto imports = current_AST_module.imports;
        for (int i = 0; i < imports.size(); i++)
        {
            if (modules[imports[i].value].functions.find(name.value) != global_functions.end())
            {
                return 1;
            }
        }
    }
    return {};
}

std::optional<int> SemanticLocalScopeVisitor::find_function_local(Tokens name)
{
    return std::optional<int>();
}

std::optional<int> SemanticLocalScopeVisitor::find_function(Tokens name)
{
    auto function_global = find_function_global(name);
    if (function_global.has_value())
    {
        return function_global;
    }
    auto function_local = find_function_local(name);
    if (function_local.has_value())
    {
        return function_local;
    }
    return {}; // return std::optional<int>();
}

void SemanticLocalScopeVisitor::Visit(ASTNode *node)
{
}

void SemanticLocalScopeVisitor::Visit(FunctionNode *node)
{

    for (int i = 0; i < node->stmnts.size(); i++)
    {
        node->stmnts[i]->Accept(this);
    }
}

void SemanticLocalScopeVisitor::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->functions.size(); i++)
    {
        node->functions[i]->Accept(this);
    }
}

void SemanticLocalScopeVisitor::Visit(ReturnNode *node)
{
    node->Expr->Accept(this);
}

void SemanticLocalScopeVisitor::Visit(FunctionCallNode *node)
{
    if (!this->find_function(node->name).has_value())
    {
        std::cout << "hazelc: function call " << node->name.value
                  << " is not a defined function " << std::endl;
        exit(EXIT_FAILURE);
    }
}

void SemanticLocalScopeVisitor::Visit(ProgramNode *node)
{
    for (const auto &[key, current_module] : node->avail_modules)
    {
        this->current_AST_module = modules[key];
        current_module->Accept(this);
    }
}

void SemanticLocalScopeVisitor::Visit(ExprNode *node)
{
    node->lhs->Accept(this);
    node->rhs->Accept(this);
}
