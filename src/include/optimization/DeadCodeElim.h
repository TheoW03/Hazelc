#include <Frontend/Ast.h>
#include <visitor.h>
#include <vector>
#include <queue>

#ifndef DEAD_CODE_H
#define DEAD_CODE_H

class DeadCode : public Visitor
{
private:
    bool isVisited(Tokens value);

public:
    std::vector<std::shared_ptr<FunctionNode>> f;
    std::queue<Tokens> func_calls;

    std::set<std::string> global;
    std::set<std::string> local;
    DeadCode();

    void Visit(ProgramNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ExprNode *node) override;
    void Visit(FunctionCallNode *node) override;
    void Visit(ReturnNode *node) override;
};
#endif
