#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <visitor.h>

#ifndef COMPILER_H
#define COMPILER_H
class CompileHighLevel : public Visitor
{
private:
    std::map<std::string, CompiledModuleClass> modules;

public:
    llvm::Module &module;
    std::vector<std::shared_ptr<ASTNode>> functions;

    CompilerContext compiler_context;
    std::map<std::string, Function> func_map;

    std::queue<Function> compiled_functions;
    bool is_global;

    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    CompileHighLevel(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context);
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ModuleNode *node) override;

    void Visit(ReturnNode *node) override;
    void Visit(FunctionCallNode *node) override;
    void Visit(ProgramNode *node) override;

    Function CompileFunctionHeader(std::shared_ptr<FunctionRefNode> n);
    ProgramScope getProgramScope();
};

#endif

#ifndef COMPILE_STATEMENT_H
#define COMPILE_STATEMENT_H
class CompileStatement : public Visitor
{
public:
    llvm::Module &module;
    CompiledModule current_module;
    CompilerContext compiler_context;
    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    ProgramScope program_scope;
    CompileStatement(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, CompilerContext compiler_context);
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ModuleNode *node) override;

    void Visit(ReturnNode *node) override;
    void Visit(ProgramNode *node) override;
    void Visit(FunctionCallNode *node) override;
};

#endif

#ifndef COMPILE_EXPR_H
#define COMPILE_EXPR_H
class CompileExpr
{
private:
    llvm::Value *CompileStr(llvm::Value *str, llvm::Value *length, llvm::Value *structure);
    llvm::Value *IntegerMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *FloatMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *StringMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs);

    llvm::Value *IntegerBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *FloatBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs);

public:
    llvm::Module &module;
    ProgramScope program;
    // std::map<std::string, Function> func_map;
    CompilerContext compiler_context;
    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    CompileExpr(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context,
                CompilerContext compiler_context,
                ProgramScope program);
    llvm::Value *IntMathExpression(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *FloatMathExpression(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *BoolIntMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *BoolFloatMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *StringMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *Expression(std::shared_ptr<ASTNode> node);
};
#endif
