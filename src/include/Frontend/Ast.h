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

#ifndef TYPE_H
#define TYPE_H
class Type
{
    virtual std::string to_string() = 0;
};
#endif
#ifndef NATIVE_TYPE_H
#define NATIVE_TYPE_H

class NativeType : public Type
{
    NativeType(Tokens type);
    std::string to_string();
};
#endif

#ifndef FUNCTION_TYPE_H
#define FUNCTION_TYPE_H

class FunctionType : public Type
{
    FunctionType(std::shared_ptr<Type> params, std::shared_ptr<Type> returnType);
    std::string to_string();
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

#ifndef FUNCTION_REF_NODE_H
#define FUNCTION_REF_NODE_H
class FunctionRefNode : public ASTNode
{
public:
    int number;
    FunctionRefNode(Tokens name,
                    std::vector<std::shared_ptr<FunctionRefNode>> params,
                    std::shared_ptr<Type> returnType);
    void Accept();
    std::string to_string();
};
#endif

#ifndef FUNCTION_NODE_H
#define FUNCTION_NODE_H
class FunctionNode : public ASTNode
{
public:
    FunctionNode(std::shared_ptr<FunctionRefNode> functionHeader,
                 std::vector<std::shared_ptr<ASTNode>> stmnts);
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
