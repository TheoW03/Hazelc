
#include <visitor.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <backend/Scope.h>
#include <backend/CompilerContext.h>
#include <backend/CompilerUtil.h>

#ifndef COMPILER_H
#define COMPILER_H
class CompileHighLevel : public Visitor
{
private:
    // std::map<std::string, CompiledModuleClass> modules;

public:
    llvm::Module &module;
    std::vector<llvm::Type *> params_struct;
    // llvm::StructType *paramType;
    CompilerContext compiler_context;
    std::map<std::string, Function> func_map;
    std::map<std::string, Function> exported_func_map;
    std::stack<Function> compiled_functions;
    bool is_global;
    llvm::StructType *params;

    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    CompileHighLevel(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context);
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    // void Visit(ModuleNode *node) override;
    void Visit(BranchNode *node) override;
    void Visit(ConditionalNode *node) override;
    void Visit(ReturnNode *node) override;
    void Visit(FunctionCallNode *node) override;
    // void Visit(ProgramNode *node) override;
    void Visit(DemoduarlizedProgramNode *node) override;

    void Visit(ExprNode *node) override;
    void Visit(BlockNode *node) override;

    std::shared_ptr<CompiledFunction> CompileFunctionHeader(std::shared_ptr<FunctionRefNode> n);
    std::tuple<llvm::FunctionType *, std::vector<Thunks>> compile_Function_Type(std::shared_ptr<FunctionRefNode> n);
    Thunks get_thunk_types(std::shared_ptr<FunctionRefNode> n);
};

#endif

#ifndef COMPILE_STATEMENT_H
#define COMPILE_STATEMENT_H
class CompileStatement : public Visitor
{
private:
    llvm::BasicBlock *block;

public:
    llvm::Module &module;
    CompiledModule current_module;
    CompilerContext compiler_context;
    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    // ProgramScope program_scope;
    llvm::StructType *params;
    CompileStatement(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, CompilerContext compiler_context, llvm::StructType *params);
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    // void Visit(ModuleNode *node) override;
    void Visit(DemoduarlizedProgramNode *node) override;

    void Visit(ReturnNode *node) override;
    // void Visit(ProgramNode *node) override;
};

#endif

#ifndef PHI_NODE_STRUCTURE
#define PHI_NODE_STRUCTURE
struct ValueStruct
{
    llvm::BasicBlock *block;
    llvm::Value *value;
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
    llvm::Value *StringBoolMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs);

    llvm::Value *IntegerBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *FloatBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *BoolBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs);

    llvm::Value *NoneBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs, BooleanExprNode *expr);
    ValueStruct CompileBranch(std::shared_ptr<BlockNode> stmnts);

    ValueStruct CompileConditional(ConditionalNode *codnition_stmnt);
    // PhiNodeStruct HandleConditional(std::shared_ptr<ASTNode> node);

public:
    llvm::Module &module;
    llvm::BasicBlock *block;
    llvm::StructType *params;
    // std::map<std::string, Function> func_map;
    CompilerContext compiler_context;
    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    CompileExpr(llvm::Module &module,
                llvm::IRBuilder<> &builder,
                llvm::LLVMContext &context,
                CompilerContext compiler_context, llvm::BasicBlock *block, llvm::StructType *params);
    llvm::Value *IntMathExpression(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *FloatMathExpression(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *BoolIntMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *BoolFloatMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *BoolBoolExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *StringMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);
    llvm::Value *StringBoolMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs);

    ValueStruct Expression(std::shared_ptr<ASTNode> node);
};
#endif
