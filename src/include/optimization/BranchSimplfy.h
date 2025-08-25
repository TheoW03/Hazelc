#include <Frontend/Ast.h>
#include <visitor.h>

#ifndef BRANCH_SIMPLFY_VISITOR
#define BRANCH_SIMPLFY_VISITOR
class BranchSimplfyVisitor : public Visitor
{
public:
    BranchSimplfyVisitor();

    void Visit(DemoduarlizedProgramNode *node) override;

    // void Visit(ProgramNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(BlockNode *node) override;

    std::shared_ptr<ASTNode> substitute(std::shared_ptr<ASTNode>);
};
#endif