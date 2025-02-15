#include <visitor.h>

void SemanticAnalysisVisit::Visit(ASTNode *node)
{
}
void SemanticAnalysisVisit::Visit(FunctionNode *node)
{
}
void SemanticAnalysisVisit::Visit(ModuleNode *node)
{
}
void SemanticAnalysisVisit::Visit(ReturnNode *node)
{
}

void SemanticAnalysisVisit::Visit(FunctionCallNode *node)
{
}

void SemanticAnalysisVisit::Visit(ProgramNode *node)
{
    // for (int i = 0; i < node->modules.size(); i++)
    // {
    // }
}
