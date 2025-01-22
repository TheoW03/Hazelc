

#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

#include <Frontend/Ast.h>
#include <map>
#include <memory>
#include <backend/CompilerUtil.h>

#ifndef VISITOR_H
#define VISITOR_H
class Visitor
{
public:
    Visitor();
    ~Visitor();
    virtual void Visit(ASTNode *node) = 0;
    virtual void Visit(ModuleNode *node) = 0;
    virtual void Visit(FunctionNode *node) = 0;
    virtual void Visit(ReturnNode *node) = 0;
};
#endif

#ifndef SEMANTIC_H
#define SEMANTIC_H
class SemanticAnalysisVisit : public Visitor
{
public:
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ModuleNode *node) override;
    void Visit(ReturnNode *node) override;
};
#endif
#ifndef COMPILER_H
#define COMPILER_H
class CompileHighLevel : public Visitor
{
public:
    llvm::Module &module;
    std::map<std::string, Function> func_map;
    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    CompileHighLevel(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context);
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ModuleNode *node) override;

    void Visit(ReturnNode *node) override;

    Function CompileFunctionHeader(std::shared_ptr<FunctionRefNode> n);
    std::map<std::string, Function> get_map();
};

#endif
#ifndef COMPILE_STATEMENT_H
#define COMPILE_STATEMENT_H
class CompileStatement : public Visitor
{
public:
    llvm::Module &module;
    std::map<std::string, Function> func_map;
    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    CompileStatement(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::map<std::string, Function> func_map);
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ModuleNode *node) override;

    void Visit(ReturnNode *node) override;
};

#endif

#ifndef COMPILE_EXPR_H
#define COMPILE_EXPR_H
class CompileExpr
{
public:
    llvm::Module &module;
    std::map<std::string, Function> func_map;
    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    CompileExpr(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::map<std::string, Function> func_map);
    llvm::Value *IntMathExpression(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *FloatMathExpression(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *BoolIntMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *BoolFloatMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);

    llvm::Value *Expression(std::shared_ptr<ASTNode> node);
};
#endif