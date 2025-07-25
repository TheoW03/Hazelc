#include <Frontend/SemanticCheckScopes.h>
#include <map>
#include <error.h>
// pass to resolve global scope
// it creates a list data structure of the different modules and there imports
// so we can then validate the local scope
void SemanticGlobalScopeVisitor::Visit(ASTNode *node)
{
}
bool SemanticGlobalScopeVisitor::function_exists(Tokens name)
{
    if (module_functions.find(name.value) != module_functions.end())
    {
        return true;
    }
    else
    {
        for (int i = 0; i < current->imports.size(); i++)
        {
            if (this->modules.find(current->imports[i].value) != this->modules.end())
            {
                auto f = this->modules[current->imports[i].value];
                if (f.exported_functions.find(name.value) != f.exported_functions.end())
                {
                    return true;
                }
            }
            else
            {
                auto c = this->avail_modules[current->imports[i].value];
                for (const auto &function : c->functions)
                {
                    if (function->f->FunctionName.value == name.value && function->can_export)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
void SemanticGlobalScopeVisitor::Visit(FunctionNode *node)
{
    if (!function_exists(node->f->FunctionName))
    {

        module_functions.insert(std::make_pair(node->f->FunctionName.value, (node->f)));
    }
    else
    {
        error("repeating function name, " + node->f->FunctionName.value, node->f->FunctionName);
        // std::cout << "repeating function name, \"" << node->f->FunctionName.value << "\"" << std::endl;
        // exit(EXIT_FAILURE);
    }
    if (node->can_export)
    {
        this->exported_functions.insert(std::make_pair(node->f->FunctionName.value, (node->f)));
    }
    // std::map<std::string, std::vector<SemanticFunction>> func;
    // // SemanticFunction f = {no};
    // std::vector<SemanticFunction> functions;
    // functions.push_back(f);
    // auto c = std::make_pair(node->f->FunctionName.value, functions);
    // this->current_AST_module.functions.insert(c);
    // if (node->can_export == true)
    // {
    //     this->current_AST_module.exported_functions.insert(c);
    // }
}

void SemanticGlobalScopeVisitor::Visit(ModuleNode *node)
{

    for (int i = 0; i < node->functions.size(); i++)
    {
        node->functions[i]->Accept(this);
    }
}

void SemanticGlobalScopeVisitor::Visit(ReturnNode *node)
{
}

void SemanticGlobalScopeVisitor::Visit(FunctionCallNode *node)
{
}

void SemanticGlobalScopeVisitor::Visit(ProgramNode *node)
{
    this->avail_modules = node->avail_modules;
    for (const auto &[key, current_module] : node->avail_modules)
    {
        SemanticModule c;
        // this->current_AST_module = c;
        this->current = current_module;
        current_module->Accept(this);
        c = {current_module->name, this->module_functions, this->exported_functions, current_module->imports};

        // this->modules.push_back(c);
        this->modules.insert(std::make_pair(key, c));
        this->module_functions.clear();
        this->exported_functions.clear();
    }
    std::cout << "hazelc: Resolved Global Scope" << std::endl;
}
