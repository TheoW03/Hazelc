#include <visitor.h>

void SemanticAnalysisTopLevel::Visit(ASTNode *node)
{
}

void SemanticAnalysisTopLevel::Visit(FunctionNode *node)
{
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
        current_module->Accept(this);
    }
}
