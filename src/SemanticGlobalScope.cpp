#include <Frontend/SemanticCheckScopes.h>
#include <map>

// pass to resolve global scope
// it creates a list data structure of the different modules and there imports
// so we can then validate the local scope
void SemanticGlobalScopeVisitor::Visit(ASTNode *node)
{
}

void SemanticGlobalScopeVisitor::Visit(FunctionNode *node)
{
    if (module_functions.find(node->f->FunctionName.value) == module_functions.end())
    {

        module_functions.insert(std::make_pair(node->f->FunctionName.value, (node)));
    }
    else
    {
        std::cout << "repeating function name, \"" << node->f->FunctionName.value << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (node->can_export)
    {
        this->exported_functions.insert(std::make_pair(node->f->FunctionName.value, (node)));
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
        current_module->Accept(this);
        c = {current_module->name, this->module_functions, this->exported_functions, current_module->imports};

        // this->modules.push_back(c);
        this->modules.insert(std::make_pair(key, c));
        this->module_functions.clear();
        this->exported_functions.clear();
    }
    // DEBUG
    // for (int i = 0; i < this->modules.size(); i++)
    // {
    //     for (const auto &[key, current_module] : this->modules[i].functions)
    //     {
    //         std::cout << "local: " << key << std::endl;
    //     }
    //     for (const auto &[key, current_module] : this->modules[i].exported_functions)
    //     {
    //         std::cout << "export: " << key << std::endl;
    //     }
    // }
}
