#include <Frontend/lexxer.h>
#include <iostream>
#include <memory>
#include <map>
#include <optional>
#ifndef ASTNODE_H
#define ASTNODE_H
class Visitor;
class ASTNode
{
public:
    // Visitor v;
    virtual void Accept(Visitor *v) = 0;
    ASTNode();
    ~ASTNode();
    virtual std::string to_string() = 0;
};
#endif
#ifndef CALL_H
#define CALL_H
class Visitor;
class FunctionCallNode : public ASTNode
{
public:
    Tokens name;
    std::vector<std::shared_ptr<ASTNode>> params;
    FunctionCallNode(Tokens name, std::vector<std::shared_ptr<ASTNode>> params);
    void Accept(Visitor *v) override;
    std::string to_string();
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
    virtual std::string get_type_value() = 0;
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
    std::string get_type_value() override;

    std::string to_string();
};
#endif

#ifndef FUNCTION_TYPE_H
#define FUNCTION_TYPE_H

class FunctionType : public Type
{
public:
    FunctionType(std::vector<std::shared_ptr<Type>> params, std::shared_ptr<Type> returnType);
    std::string get_type_value() override;
    std::string to_string();
};
#endif

#ifndef LIST_TYPE_H
#define LIST_TYPE_H

class ListType : public Type
{
public:
    std::shared_ptr<Type> inner_type;
    ListType(std::shared_ptr<Type> inner);
    std::string get_type_value() override;

    std::string to_string();
};
#endif

#ifndef INTNODE_H
#define INTNODE_H
class Visitor;
class IntegerNode : public ASTNode
{
public:
    int number;

    IntegerNode(Tokens num);
    IntegerNode(int num);
    void Accept(Visitor *v);
    std::string to_string();
};
#endif

#ifndef DECIMAL_H
#define DECIMAL_H
class Visitor;
class DecimalNode : public ASTNode
{
public:
    double number;
    DecimalNode(Tokens num);
    DecimalNode(double num);
    void Accept(Visitor *v);
    std::string to_string();
};
#endif

#ifndef BOOL_EXPR_NODe_H
#define BOOL_EXPR_NODe_H
class Visitor;
class BooleanExprNode : public ASTNode
{
public:
    std::shared_ptr<ASTNode> lhs;
    std::shared_ptr<ASTNode> rhs;
    Tokens op;
    BooleanExprNode(std::shared_ptr<ASTNode> lhs, Tokens operation, std::shared_ptr<ASTNode> rhs);
    void Accept(Visitor *v);
    std::string to_string();
};
#endif
#ifndef BOOL_CONST_NODE_H
#define BOOL_CONST_NODE_H
class Visitor;
class BooleanConstNode : public ASTNode
{
public:
    bool val;
    BooleanConstNode(Tokens value);
    BooleanConstNode(bool value);

    void Accept(Visitor *v);
    std::string to_string();
};
#endif
#ifndef FUNCTION_REF_NODE_H
#define FUNCTION_REF_NODE_H
class FunctionRefNode : public ASTNode
{
public:
    Tokens FunctionName;
    std::vector<std::shared_ptr<FunctionRefNode>> params;
    std::shared_ptr<Type>
        RetType;
    FunctionRefNode(
        Tokens name,
        std::vector<std::shared_ptr<FunctionRefNode>> params,
        std::shared_ptr<Type> returnType);
    void Accept(Visitor *v);
    std::string to_string();
};
#endif

#ifndef FUNCTION_NODE_H
#define FUNCTION_NODE_H
class Visitor;

class FunctionNode : public ASTNode
{
public:
    std::shared_ptr<FunctionRefNode> f;
    std::vector<std::shared_ptr<ASTNode>> stmnts;
    bool can_export;
    FunctionNode(bool can_export,
                 std::shared_ptr<FunctionRefNode> functionHeader,
                 std::vector<std::shared_ptr<ASTNode>> stmnts);
    void Accept(Visitor *v);
    std::string to_string();
};
#endif

#ifndef EXPR_NODE_H
#define EXPR_NODE_H
class ExprNode : public ASTNode
{
private:
    // std::shared_ptr<ASTNode> fold(std::shared_ptr<ASTNode> node);

public:
    // std::shared_ptr<ASTNode> fold();
    std::shared_ptr<ASTNode> lhs;
    std::shared_ptr<ASTNode> rhs;
    Tokens operation;
    ExprNode(std::shared_ptr<ASTNode> lhs, Tokens operation, std::shared_ptr<ASTNode> rhs);
    std::shared_ptr<ASTNode> fold();
    void Accept(Visitor *v);
    std::string to_string();
};
#endif

#ifndef NONE_H
#define NONE_H
class Visitor;

class NoneNode : public ASTNode
{
public:
    NoneNode();
    void Accept(Visitor *v);
    std::string to_string();
};
#endif

#ifndef RETURN_H
#define RETURN_H
class Visitor;

class ReturnNode : public ASTNode
{
public:
    std::shared_ptr<ASTNode> Expr;
    ReturnNode(std::shared_ptr<ASTNode> expr);
    void Accept(Visitor *v) override;
    std::string to_string();
};
#endif

#ifndef MODULE_H
#define MODULE_H
class Visitor;

class ModuleNode : public ASTNode
{
public:
    std::vector<std::shared_ptr<ASTNode>> functions;
    Tokens name;
    std::vector<Tokens> imports;
    ModuleNode(std::vector<std::shared_ptr<ASTNode>> functions, Tokens name, std::vector<Tokens> imports);
    void Accept(Visitor *v);
    std::string to_string();
};
#endif

#ifndef PROGRAM_H
#define PROGRAM_H
class Visitor;

class ProgramNode : public ASTNode
{

public:
    std::map<std::string, std::shared_ptr<ModuleNode>> avail_modules;
    std::map<std::string, std::shared_ptr<ModuleNode>> used_modules;

    ProgramNode(std::map<std::string, std::shared_ptr<ModuleNode>> modules);
    void Accept(Visitor *v);
    std::optional<std::shared_ptr<ModuleNode>> getMainModule();
    std::string to_string();
};
#endif

#ifndef LIST_NODE_H
#define LIST_NODE_H
class Visitor;

class ListNode : public ASTNode
{
public:
    std::vector<std::shared_ptr<ASTNode>> values;
    ListNode(std::vector<std::shared_ptr<ASTNode>> values);
    void Accept(Visitor *v);
    std::string to_string();
};
#endif
#ifndef STRING_NODE_H
#define STRING_NODE_H
class Visitor;

class StringNode : public ASTNode
{
public:
    // Tokens value;
    std::string value;
    StringNode(Tokens value);
    StringNode(std::string value);

    void Accept(Visitor *v);
    std::string to_string();
};
#endif

#ifndef CHAR_NODE_H
#define CHAR_NODE_H
class Visitor;

class CharNode : public ASTNode
{
public:
    Tokens value;
    CharNode(Tokens value);
    void Accept(Visitor *v);
    std::string to_string();
};
#endif

#ifndef BRANCH_NODE_H
#define BRANCH_NODE_H
class Visitor;
class BranchNode : public ASTNode
{
public:
    BranchNode();
    std::shared_ptr<ASTNode> condition;
    std::vector<std::shared_ptr<ASTNode>> stmnts;
    BranchNode(std::shared_ptr<ASTNode> condition, std::vector<std::shared_ptr<ASTNode>> stmnts);
    void Accept(Visitor *v) override;
    std::string to_string();
};
#endif

#ifndef CONDITIONAL_H
#define CONDITIONAL_H

class Visitor;
class ConditionalNode : public ASTNode
{
public:
    std::vector<std::shared_ptr<BranchNode>> branches;
    std::shared_ptr<Type> type;
    ConditionalNode();
    ConditionalNode(std::vector<std::shared_ptr<BranchNode>> branches,
                    std::shared_ptr<Type> type);
    void Accept(Visitor *v) override;
    std::string to_string();
};
#endif
