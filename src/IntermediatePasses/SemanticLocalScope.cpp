#include <Frontend/SemanticCheckScopes.h>
#include <error.h>
#include <sha256.h>
SemanticLocalScopeVisitor::SemanticLocalScopeVisitor(std::map<std::string, SemanticModule> modules)
{
    this->modules = modules;
}

// these find functions, optional so we can do more stuff with them

std::optional<bool> SemanticLocalScopeVisitor::find_function_global(Tokens name)
{
    auto global_functions = this->current_AST_module.functions;
    if (global_functions.find(name.value) != global_functions.end())
    {
        return true;
    }
    else
    {
        auto imports = current_AST_module.imports;
        for (int i = 0; i < imports.size(); i++)
        {
            if (modules[imports[i].value].exported_functions.find(name.value) != modules[imports[i].value].exported_functions.end())
            {

                return true;
            }
        }
    }
    return {};
}

std::optional<bool> SemanticLocalScopeVisitor::find_function_local(Tokens name)
{
    for (int i = 0; i < scope.size(); i++)
    {
        if (scope[i].functions.find(name.value) != scope[i].functions.end())
        {
            return true;
        }
    }
    return {};
}

std::optional<std::shared_ptr<FunctionRefNode>> SemanticLocalScopeVisitor::get_local_function(Tokens name)
{
    for (int i = 0; i < scope.size(); i++)
    {
        if (scope[i].functions.find(name.value) != scope[i].functions.end())
        {
            return scope[i].functions[name.value];
        }
    }
    return {};
}
std::optional<std::shared_ptr<FunctionRefNode>> SemanticLocalScopeVisitor::get_global_function(Tokens name)
{
    auto global_functions = this->current_AST_module.functions;
    if (global_functions.find(name.value) != global_functions.end())
    {
        // FastLookup f = {
        // current_AST_module.name,
        // name};
        // return f;
        return global_functions[name.value];
    }
    else
    {
        auto imports = current_AST_module.imports;
        for (int i = 0; i < imports.size(); i++)
        {
            if (modules[imports[i].value].exported_functions.find(name.value) != modules[imports[i].value].exported_functions.end())
            {
                return modules[imports[i].value].exported_functions[name.value];
            }
        }
    }
    return {};
}
std::optional<std::shared_ptr<FunctionRefNode>> SemanticLocalScopeVisitor::get_function(Tokens name)
{
    auto function_global = get_global_function(name);
    if (function_global.has_value())
    {
        return function_global;
    }
    auto function_local = get_local_function(name);
    if (function_local.has_value())
    {
        return function_local;
    }
    return {}; // return std::optional<int>();
}
std::optional<bool> SemanticLocalScopeVisitor::find_function(Tokens name)
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

bool SemanticLocalScopeVisitor::function_is_param(Tokens name)
{
    for (int i = 0; i < scope.size(); i++)
    {
        if (scope[i].params.find(name.value) != scope[i].params.end())
        {
            return true;
        }
    }
    return false;
}
void SemanticLocalScopeVisitor::Visit(ASTNode *node)
{
}

void SemanticLocalScopeVisitor::Visit(FunctionNode *node)
{
    if ((find_function_global(node->f->FunctionName).has_value() && scope.size() != 0) //
        || find_function_local(node->f->FunctionName).has_value())
    {
        // std::cout << "there is an already defined function \"" << node->f->FunctionName.value << "\"" << node->f->FunctionName.line_num << std::endl;
        // exit(EXIT_FAILURE);

        error("repeating function name, " + node->f->FunctionName.value, node->f->FunctionName);
    }
    if (scope.size() >= 1)
    {
        scope[scope.size() - 1].functions.insert(std::make_pair(node->f->FunctionName.value, (node->f)));
        // scope[scope.size() - 1].functions.insert(node->f->FunctionName.value);
    }
    FunctionLocalScope f;
    scope.push_back(f);
    for (int i = 0; i < node->f->params.size(); i++)
    {
        scope[scope.size() - 1].functions.insert(std::make_pair(node->f->params[i]->FunctionName.value,
                                                                (node->f->params[i])));
        scope[scope.size() - 1].params.insert(std::make_pair(node->f->params[i]->FunctionName.value,
                                                             (node->f->params[i])));
    }

    // for (int i = 0; i < node->stmnts.size(); i++)
    // {
    //     node->stmnts[i]->Accept(this);
    // }
    node->stmnts->Accept(this);
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
        error("undefined function call," + node->name.value, node->name);
    }
    node->param = function_is_param(node->name);
    for (int i = 0; i < node->params.size(); i++)
    {
        node->params[i]->Accept(this);
    }
    node->param_types = this->get_function(node->name).value()->params;
    // std::cout << node->ident.ident_name.value().value << std::endl;
}

void SemanticLocalScopeVisitor::Visit(ProgramNode *node)
{
    for (const auto &[key, current_module] : node->avail_modules)
    {
        this->current_AST_module = modules[key];
        current_module->Accept(this);
    }
    std::cout << "hazelc: Resolved Local Scope" << std::endl;
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
    // for (int i = 0; i < node->stmnts.size(); i++)
    // {
    //     node->stmnts[i]->Accept(this);
    // }
    node->stmnts->Accept(this);
    scope.erase(scope.end() - 1);
}

void SemanticLocalScopeVisitor::Visit(BlockNode *node)
{
    for (int i = 0; i < node->functions.size(); i++)
    {
        node->functions[i]->Accept(this);
    }
    node->exit->Accept(this);
}

IntermediateScope::IntermediateScope()
{
}

IntermediateScope::IntermediateScope(std::map<std::string, SemanticModule> modules)
{
    this->modules = modules;
}
IntermediateScope::IntermediateScope(std::unordered_map<std::string, std::shared_ptr<FunctionNode>> functions)
{
    this->functions = functions;
}

void IntermediateScope::set_current_module(std::string s)

{
    this->current = this->modules[s];
}
std::optional<SemanticModule> IntermediateScope::getModuleNameByFunction(Tokens name)
{

    auto global_functions = this->current.functions;
    if (global_functions.find(name.value) != global_functions.end())
    {

        return this->current;
    }
    else
    {
        auto imports = current.imports;
        for (int i = 0; i < imports.size(); i++)
        {
            if (modules[imports[i].value].exported_functions.find(name.value) != modules[imports[i].value].exported_functions.end())
            {
                return modules[imports[i].value];
            }
        }
    }
    return {};
    // return SemanticModule();
}
std::optional<std::shared_ptr<FunctionRefNode>> IntermediateScope::get_global_function(Tokens name)
{
    auto global_functions = current.functions;
    if (global_functions.find(name.value) != global_functions.end())
    {
        return global_functions[name.value];
    }
    else
    {
        auto imports = current.imports;
        for (int i = 0; i < imports.size(); i++)
        {
            if (modules[imports[i].value].exported_functions.find(name.value) != modules[imports[i].value].exported_functions.end())
            {
                return modules[imports[i].value].exported_functions[name.value];
            }
        }
    }
    return {};
}
std::optional<std::shared_ptr<FunctionRefNode>> IntermediateScope::get_global_function_demodularized(std::optional<std::string> function_name)
{
    if (!function_name.has_value())
    {
        return {};
    }
    if (functions.find(function_name.value()) != this->functions.end())
    {
        return functions[function_name.value()]->f;
    }
    return {};
}
std::optional<std::string> IntermediateScope::get_global_function_hash(Tokens function_name)
{
    if (!get_global_function(function_name).has_value())
        return {};
    auto new_name = function_name.value + ":" + getModuleNameByFunction(function_name).value().name.value;
    return new_name;
}