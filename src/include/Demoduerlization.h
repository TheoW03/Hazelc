#include <iostream>
#include <visitor.h>
#include <Frontend/SemanticCheckScopes.h>

#ifndef DEMODULERIZATION_VISTOR_H
#define DEMODULERIZATION_VISTOR_H

class DemodularizedVisitor : public Visitor
{
public:
    DemoduarlizedProgramNode program;
    IntermediateScope modules;
    DemodularizedVisitor(IntermediateScope s);
    void Visit(FunctionNode *node) override;
    void Visit(ModuleNode *node) override;
    void Visit(ProgramNode *node) override;
    // void Visit(ProgramNode *node) override;
    void Visit(ExprNode *node) override;
    void Visit(BooleanExprNode *node) override;
    void Visit(ConditionalNode *node) override;

    void Visit(FunctionCallNode *node) override;
};
#endif