
#include <visitor.h>

#ifndef INLINE_H
#define INLINE_H
class InlineTopLevelVisitor : public Visitor
{
private:
    std::map<std::string, std::shared_ptr<ReturnNode>> functions;

public:
    void Visit(DemoduarlizedProgramNode *node) override;
    void Visit(FunctionNode *node) override;
    std::shared_ptr<ReturnNode> getFunction(std::shared_ptr<ASTNode>);

    std::shared_ptr<ASTNode> substitute(std::shared_ptr<ASTNode>);
};
#endif
