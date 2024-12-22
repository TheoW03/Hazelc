#include <Frontend/lexxer.h>
#include <memory>
#include <iostream>

#ifndef ASTNODE_H
#define ASTNODE_H
class ASTNode
{
public:
    virtual void Accept() = 0;
    ASTNode();
    ~ASTNode();
    virtual std::string to_string() = 0;
};
#endif

#ifndef INTNODE_H
#define INTNODE_H
class IntegerNode : public ASTNode
{
public:
    int number;

    IntegerNode(Tokens num);
    void Accept();
    std::string to_string();
};
#endif

#ifndef EXPR_NODE_H
#define EXPR_NODE_H
class ExprNode : public ASTNode
{
public:
    std::shared_ptr<ASTNode> lhs;
    std::shared_ptr<ASTNode> rhs;
    Tokens operation;
    ExprNode(std::shared_ptr<ASTNode> lhs, Tokens operation, std::shared_ptr<ASTNode> rhs);
    void Accept();
    std::string to_string();
};
#endif