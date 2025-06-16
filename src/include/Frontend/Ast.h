#include <Frontend/lexxer.h>
#include <iostream>
#include <memory>
#include <map>
#include <optional>
#include <error.h>
// #ifndef IDENT_H
// #define IDENT_H
// struct FastLookup
// {
//     std::optional<Tokens> module_name;
//     std::optional<Tokens> ident_name;
// };
// #endif

#ifndef ASTNODE_H
#define ASTNODE_H
class Visitor;
class ASTNode
{
public:
    // Visitor v;

    NodeLocation node;
    virtual void Accept(Visitor *v) = 0;
    ASTNode();
    ASTNode(NodeLocation node);

    ~ASTNode();
    virtual std::string to_string() = 0;
};
#endif

// #ifndef CALL_H
// #define CALL_H
// class Visitor;
// class FunctionCallNode : public ASTNode
// {
// public:
//     FastLookup ident;
//     Tokens name;
//     std::vector<std::shared_ptr<FunctionRefNode>> param_types;
//     std::vector<std::shared_ptr<ASTNode>> params;
//     FunctionCallNode(Tokens name, std::vector<std::shared_ptr<ASTNode>> params);
//     void Accept(Visitor *v) override;
//     std::string to_string();
// };
// #endif

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
    virtual bool can_accept(Type *type) = 0;
};

#endif
#ifndef INTEGER_TYPE_H
#define INTEGER_TYPE_H
class IntegerType : public Type
{
public:
    bool is_unsigned;
    IntegerType(bool is_unsigned);
    std::string get_type_value() override;
    std::string to_string() override;

    bool can_accept(Type *type) override;
};
#endif

#ifndef BYTE_TYPE_H
#define BYTE_TYPE_H
class ByteType : public Type
{
public:
    bool is_unsigned;

    ByteType(bool is_unsigned);
    std::string get_type_value() override;
    std::string to_string() override;

    bool can_accept(Type *type) override;
};
#endif

#ifndef BOOL_TYPE_H
#define BOOL_TYPE_H
class BoolType : public Type
{
public:
    BoolType();
    std::string get_type_value() override;
    std::string to_string() override;

    bool can_accept(Type *type) override;
};
#endif

#ifndef CHAR_TYPE_H
#define CHAR_TYPE_H
class CharacterType : public Type
{
public:
    CharacterType();
    std::string get_type_value() override;
    std::string to_string() override;

    bool can_accept(Type *type) override;
};
#endif

#ifndef DECIMAL_TYPE_H
#define DECIMAL_TYPE_H
class DecimalType : public Type
{
public:
    DecimalType();
    std::string get_type_value() override;
    std::string to_string() override;

    bool can_accept(Type *type) override;
};
#endif

#ifndef STRING_TYPE_H
#define STRING_TYPE_H
class StringType : public Type
{
public:
    StringType();
    std::string get_type_value() override;
    bool can_accept(Type *type) override;
    std::string to_string() override;
};
#endif

// #ifndef NATIVE_TYPE_H
// #define NATIVE_TYPE_H

// class NativeType : public Type
// {
// public:
//     Tokens type;
//     NativeType(Tokens type);
//     NativeType(TokenType t);
//     std::string get_type_value() override;
//     bool can_accept(Type *type) override;
//     std::string to_string();
// };
// #endif

#ifndef FUNCTION_TYPE_H
#define FUNCTION_TYPE_H

class FunctionType : public Type
{
public:
    std::vector<std::shared_ptr<Type>> params;
    std::shared_ptr<Type> returnType;
    FunctionType(std::vector<std::shared_ptr<Type>> params, std::shared_ptr<Type> returnType);
    std::string get_type_value() override;
    bool can_accept(Type *type) override;

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
    bool can_accept(Type *type) override;

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
    IntegerNode(Tokens num, NodeLocation location);

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

    std::shared_ptr<Type> get_func_type();
    void Accept(Visitor *v) override;
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

#ifndef BLOCK_NODE_H
#define BLOCK_NODE_H

class Visitor;
class FunctionNode;

class BlockNode : public ASTNode
{
public:
    std::vector<std::shared_ptr<FunctionNode>> functions;
    std::shared_ptr<ReturnNode> exit;
    BlockNode();
    BlockNode(std::vector<std::shared_ptr<FunctionNode>> functions,
              std::shared_ptr<ReturnNode> exit);
    void Accept(Visitor *v) override;
    std::string to_string();
};
#endif

#ifndef FUNCTION_NODE_H
#define FUNCTION_NODE_H
class Visitor;

class FunctionNode : public ASTNode
{
public:
    // FastLookup ident;
    std::shared_ptr<FunctionRefNode> f;
    std::shared_ptr<BlockNode> stmnts;
    bool can_export;
    std::string hash_name;
    FunctionNode(bool can_export,
                 std::shared_ptr<FunctionRefNode> functionHeader,
                 std::shared_ptr<BlockNode>);
    void Accept(Visitor *v) override;
    std::string to_string();
};
#endif

#ifndef CALL_H
#define CALL_H
class Visitor;
class FunctionCallNode : public ASTNode
{
public:
    Tokens name;
    std::string hash_name;
    std::vector<std::shared_ptr<FunctionRefNode>> param_types;
    std::vector<std::shared_ptr<ASTNode>> params;
    FunctionCallNode(Tokens name, std::vector<std::shared_ptr<ASTNode>> params);
    void Accept(Visitor *v) override;
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
    void Accept(Visitor *v) override;
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
    std::vector<std::shared_ptr<FunctionNode>> functions;
    Tokens name;
    std::vector<Tokens> imports;
    ModuleNode(std::vector<std::shared_ptr<FunctionNode>> functions, Tokens name, std::vector<Tokens> imports);
    void Accept(Visitor *v) override;
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
    void Accept(Visitor *v) override;
    std::optional<std::shared_ptr<ModuleNode>> getMainModule();
    std::optional<std::shared_ptr<FunctionNode>> getMainFunction();
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
    void Accept(Visitor *v) override;
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

    void Accept(Visitor *v) override;
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
    void Accept(Visitor *v) override;
    std::string to_string();
};
#endif

#ifndef BRANCH_NODE_H
#define BRANCH_NODE_H
class Visitor;
class BranchNode : public ASTNode
{
public:
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<BlockNode> stmnts;
    BranchNode();
    BranchNode(std::shared_ptr<ASTNode> condition, std::shared_ptr<BlockNode> stmnts);
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
                    std::shared_ptr<Type> type, NodeLocation location);
    void Accept(Visitor *v) override;
    std::string to_string();
};
#endif

#ifndef DEMODULARIZED_PROGRAM_NODE_H
#define DEMODULARZIED_PROGRAM_NODE_H
#pragma once
class Visitor;

class DemoduarlizedProgramNode : public ASTNode
{
public:
    std::map<std::string, std::shared_ptr<FunctionNode>> global_functions;
    std::map<std::string, std::shared_ptr<ModuleNode>> avail_modules;
    std::map<std::string, std::shared_ptr<ModuleNode>> used_modules;

    std::vector<std::shared_ptr<FunctionNode>> functions;
    DemoduarlizedProgramNode();
    void Accept(Visitor *v) override;
    std::string to_string() override;
};
#endif