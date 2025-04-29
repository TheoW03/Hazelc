#include <visitor.h>
#include <backend/CompilerUtil.h>
Visitor::Visitor()
{
}

Visitor::~Visitor()
{
}
void Visitor::Visit(ASTNode *node)
{
}
void Visitor::Visit(ModuleNode *node)
{
}
void Visitor::Visit(FunctionNode *node)
{
}

void Visitor::Visit(ReturnNode *node)
{
}

void Visitor::Visit(FunctionCallNode *node)
{
}

void Visitor::Visit(ProgramNode *node)
{
}

void Visitor::Visit(IntegerNode *node)
{
}

void Visitor::Visit(DecimalNode *node)
{
}

void Visitor::Visit(CharNode *node)
{
}

void Visitor::Visit(StringNode *node)
{
}

void Visitor::Visit(BooleanExprNode *node)
{
}

void Visitor::Visit(BooleanConstNode *node)
{
}

void Visitor::Visit(ExprNode *node)
{
}

void Visitor::Visit(ConditionalNode *node)
{
}

void Visitor::Visit(BranchNode *node)
{
}

void Visitor::Visit(BlockNode *node)
{
}

void Visitor::Visit(ListNode *node)
{
}
