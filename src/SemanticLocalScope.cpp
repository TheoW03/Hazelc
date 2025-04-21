#include <Frontend/SemanticCheckScopes.h>

SemanticLocalScopeVisitor::SemanticLocalScopeVisitor(std::map<std::string, SemanticModule> modules)
{
    this->modules = modules;
}

// these find functions, optional so we can do more stuff with them

std::optional<FastLookup> SemanticLocalScopeVisitor::find_function_global(Tokens name)
{
    auto global_functions = this->current_AST_module.functions;
    if (global_functions.find(name.value) != global_functions.end())
    {
        FastLookup f = {
            current_AST_module.name,
            name};
        return f;
    }
    else
    {
        auto imports = current_AST_module.imports;
        for (int i = 0; i < imports.size(); i++)
        {
            if (modules[imports[i].value].exported_functions.find(name.value) != modules[imports[i].value].exported_functions.end())
            {

                FastLookup f = {
                    imports[i],
                    name};
                return f;
            }
        }
    }
    return {};
}

std::optional<FastLookup> SemanticLocalScopeVisitor::find_function_local(Tokens name)
{
    for (int i = 0; i < scope.size(); i++)
    {
        if (scope[i].functions.find(name.value) != scope[i].functions.end())
        {
            FastLookup f = {{}, name};
            return f;
        }
    }
    return {};
}

std::optional<FastLookup> SemanticLocalScopeVisitor::find_function(Tokens name)
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
    if ((find_function_global(node->f->FunctionName).has_value() && scope.size() != 0) //
        || find_function_local(node->f->FunctionName).has_value())
    {
        std::cout << "there is an already defined function \"" << node->f->FunctionName.value << "\"" << node->f->FunctionName.line_num << std::endl;
        exit(EXIT_FAILURE);
    }
    if (scope.size() >= 1)
    {
        scope[scope.size() - 1].functions.insert(node->f->FunctionName.value);
    }
    FunctionLocalScope f;
    scope.push_back(f);
    for (int i = 0; i < node->f->params.size(); i++)
    {
        scope[scope.size() - 1].functions.insert(node->f->params[i]->FunctionName.value);
    }
    for (int i = 0; i < node->stmnts.size(); i++)
    {
        node->stmnts[i]->Accept(this);
    }
    scope.erase(scope.end() - 1);
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
    node->ident = this->find_function(node->name).value();
    std::cout << node->ident.ident_name.value().value << std::endl;
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

void SemanticLocalScopeVisitor::Visit(BooleanExprNode *node)
{
    node->lhs->Accept(this);
    node->rhs->Accept(this);
}

void SemanticLocalScopeVisitor::Visit(ConditionalNode *node)
{
    for (int i = 0; i < node->branches.size(); i++)
    {
        node->branches[i]->Accept(this);
    }
}

void SemanticLocalScopeVisitor::Visit(BranchNode *node)
{
    FunctionLocalScope f;
    node->condition->Accept(this);
    scope.push_back(f); // allocates and deallocates scope
    for (int i = 0; i < node->stmnts.size(); i++)
    {
        node->stmnts[i]->Accept(this);
    }
    scope.erase(scope.end() - 1);
}
