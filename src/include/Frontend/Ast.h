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
public:
    // TODO: add constrctor
    Type();
    ~Type();
    virtual std::string to_string() = 0;
};
#endif
#ifndef NATIVE_TYPE_H
#define NATIVE_TYPE_H

class NativeType : public Type
{
public:
    Tokens type;
    NativeType(Tokens type);
    std::string to_string();
};
#endif

#ifndef FUNCTION_TYPE_H
#define FUNCTION_TYPE_H

class FunctionType : public Type
{
public:
    FunctionType(std::vector<std::shared_ptr<Type>> params, std::shared_ptr<Type> returnType);
    std::string to_string();
};
#endif

#ifndef LIST_TYPE_H
#define LIST_TYPE_H

class ListType : public Type
{
public:
    ListType(std::shared_ptr<Type> inner);
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
    Tokens FunctionName;
    std::shared_ptr<Type> RetType;
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
    std::shared_ptr<FunctionRefNode> f;
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

#ifndef NONE_H
#define NONE_H
class NoneNode : public ASTNode
{
public:
    NoneNode();
    void Accept();
    std::string to_string();
};
#endif
#ifndef RETURN_H
#define RETURN_H
class ReturnNode : public ASTNode
{
public:
    std::shared_ptr<ASTNode> Expr;
    ReturnNode(std::shared_ptr<ASTNode> expr);
    void Accept();
    std::string to_string();
};
#endif

#ifndef MODULE_H
#define MODULE_H
class ModuleNode : public ASTNode
{
public:
    std::vector<std::shared_ptr<ASTNode>> functions;
    Tokens name;
    ModuleNode(std::vector<std::shared_ptr<ASTNode>> functions, Tokens name);
    void Accept();
    std::string to_string();
};
#endif
