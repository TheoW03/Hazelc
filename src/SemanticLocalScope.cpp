#include <visitor.h>

SemanticLocalScopeVisitor::SemanticLocalScopeVisitor(std::map<std::string, SemanticModule> modules)
{
    this->modules = modules;
}

void SemanticLocalScopeVisitor::Visit(ASTNode *node)
{
}

void SemanticLocalScopeVisitor::Visit(FunctionNode *node)
{
}

void SemanticLocalScopeVisitor::Visit(ModuleNode *node)
{
}

void SemanticLocalScopeVisitor::Visit(ReturnNode *node)
{
}

void SemanticLocalScopeVisitor::Visit(FunctionCallNode *node)
{
}

void SemanticLocalScopeVisitor::Visit(ProgramNode *node)
{
    for (const auto &[key, current_module] : node->avail_modules)
    {
        this->current_AST_module = modules[key];
        current_module->Accept(this);
    }
}
