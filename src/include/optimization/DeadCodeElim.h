#include <Frontend/Ast.h>
#include <visitor.h>
#include <vector>
#include <queue>
#include <stack>
#include <set>

#ifndef DEAD_CODE_H
#define DEAD_CODE_H

class DeadCode : public Visitor
{
private:
    bool isVisited(std::shared_ptr<FunctionNode> node);

public:
    std::vector<std::shared_ptr<FunctionNode>> functions_referenced;
    std::vector<Tokens> func_calls;
    std::unordered_map<std::string, std::shared_ptr<FunctionNode>> functions;
    std::set<std::string> visited;
    DeadCode();
    std::stack<std::shared_ptr<FunctionNode>> workList;
    void Visit(DemoduarlizedProgramNode *node) override;

    // void Visit(ProgramNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ExprNode *node) override;
    void Visit(FunctionCallNode *node) override;
    void Visit(BlockNode *node) override;

    void Visit(ReturnNode *node) override;
};
#endif
