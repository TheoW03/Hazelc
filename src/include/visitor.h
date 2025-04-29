

// #include <backend/Scope.h>
#include <Frontend/Ast.h>
#include <map>
#include <set>
#include <optional>
#include <memory>

#ifndef VISITOR_H
#define VISITOR_H
class Visitor
{
public:
    Visitor();
    ~Visitor();
    virtual void Visit(ASTNode *node);
    virtual void Visit(ModuleNode *node);
    virtual void Visit(FunctionNode *node);
    virtual void Visit(ReturnNode *node);
    virtual void Visit(FunctionCallNode *node);
    virtual void Visit(ProgramNode *node);
    virtual void Visit(IntegerNode *node);
    virtual void Visit(DecimalNode *node);
    virtual void Visit(CharNode *node);
    virtual void Visit(StringNode *node);
    virtual void Visit(BooleanExprNode *node);
    virtual void Visit(BooleanConstNode *node);
    virtual void Visit(ExprNode *node);
    virtual void Visit(ConditionalNode *node);
    virtual void Visit(BranchNode *node);
    virtual void Visit(BlockNode *node);

    virtual void Visit(ListNode *node);
};
#endif
