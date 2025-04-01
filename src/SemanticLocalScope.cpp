#include <visitor.h>

SemanticLocalScopeVisitor::SemanticLocalScopeVisitor(std::vector<SemanticModule> modules)
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
}
