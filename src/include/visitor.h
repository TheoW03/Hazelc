

// #include "llvm/IR/Module.h"
// #include "llvm/IR/IRBuilder.h"
#include <backend/Scope.h>
#include <Frontend/Ast.h>
#include <map>
#include <memory>
#include <backend/CompilerContext.h>
#include <backend/CompilerUtil.h>

// #ifndef SEMANTUC_FUCN
// #define SEMANTUC_FUCN
// struct SemanticFunction
// {
//     std::vector<std::shared_ptr<Type>> paremeters;
//     std::shared_ptr<Type> returnType;
// };

// #endif

// #ifndef SEMANTIC_MODULE_H
// #define SEMANTIC_MODULE_H

// struct SemanticModule
// {
//     std::set<std::string> functions;
//     std::set<std::string> exported_functions;
//     std::vector<Tokens> imports;
//     // std::shared_ptr<ModuleNode> module;
//     // std::map<std::string, std::vector<SemanticFunction>> functions;
// };

// #endif

// #ifndef SEMANTIC_SCOPE
// #define SEMANTIC_SCOPE
// class Scope
// {
// public:
//     virtual void add_function(FunctionNode *f) = 0;
//     virtual SemanticFunction get_function() = 0;
// };
// #endif

// #ifndef GLOBAL_SCOPE_H
// #define GLOBAL_SCOPE_H
// class GlobalScope
// #ifndef GLOBAL_SCOPE_H
// #define GLOBAL_SCOPE_H
// class GlobalScope : public Scope
// {
// public:
//     std::map<std::string, std::vector<SemanticFunction>> functions;
//     std::map<std::string, std::vector<SemanticFunction>> exported_functions;
//     void add_function(FunctionNode *f) override;
//     SemanticFunction get_function() override;
// };
// #endif: public Scope
// {
// public:
//     std::map<std::string, std::vector<SemanticFunction>> functions;
//     std::map<std::string, std::vector<SemanticFunction>> exported_functions;
//     void add_function(FunctionNode *f) override;
//     SemanticFunction get_function() override;
// };
// #endif

#ifndef VISITOR_H
#define VISITOR_H
class Visitor
{
public:
    Visitor();
    ~Visitor();
    virtual void Visit(ASTNode *node);
    virtual void Visit(ModuleNode *node);
    virtual void Visit(FunctionNode *node);
    virtual void Visit(ReturnNode *node);
    virtual void Visit(FunctionCallNode *node);
    virtual void Visit(ProgramNode *node);
    virtual void Visit(IntegerNode *node);
    virtual void Visit(DecimalNode *node);
    virtual void Visit(CharNode *node);
    virtual void Visit(StringNode *node);
    virtual void Visit(BooleanExprNode *node);
    virtual void Visit(BooleanConstNode *node);
    virtual void Visit(ExprNode *node);
    virtual void Visit(ListNode *node);
};
#endif
