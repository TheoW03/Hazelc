#include <visitor.h>
#include <map>
void SemanticAnalysisTopLevel::Visit(ASTNode *node)
{
}

void SemanticAnalysisTopLevel::Visit(FunctionNode *node)
{
    std::map<std::string, std::vector<SemanticFunction>> func;
    SemanticFunction f = {node, func};
    std::vector<SemanticFunction> functions;
    functions.push_back(f);
    auto c = std::make_pair(node->f->FunctionName.value, functions);
    this->current_AST_module.functions.insert(c);
    if (node->can_export == true)
    {
        this->current_AST_module.exported_functions.insert(c);
    }
}

void SemanticAnalysisTopLevel::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->imports.size(); i++)
    {
        if (this->avail_modules.find(node->imports[i].value) == this->avail_modules.end())
        {
            std::cout << "invalid import \"" << node->imports[i].value << "\" in module \"" << node->name.value << "\"" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < node->functions.size(); i++)
    {
        node->functions[i]->Accept(this);
    }
}

void SemanticAnalysisTopLevel::Visit(ReturnNode *node)
{
}

void SemanticAnalysisTopLevel::Visit(FunctionCallNode *node)
{
}

void SemanticAnalysisTopLevel::Visit(ProgramNode *node)
{
    this->avail_modules = node->avail_modules;
    for (const auto &[key, current_module] : node->avail_modules)
    {
        SemanticModule c;
        this->current_AST_module = c;
        current_module->Accept(this);
        this->modules.push_back(current_AST_module);
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
