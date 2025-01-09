

#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

#include <Frontend/Ast.h>
#include <map>
#include <memory>

#ifndef VISITOR_H
#define VISITOR_H
class Visitor
{
public:
    Visitor();
    ~Visitor();
    virtual void Visit(ASTNode *node) = 0;
    virtual void Visit(FunctionNode *node) = 0;
};
#endif

#ifndef COMPILER_H
#define COMPILER_H
class CompileHighLevel : public Visitor
{
public:
    llvm::Module &module;
    std::map<std::string, llvm::Function *> func_map;
    llvm::IRBuilder<> &builder;
    llvm::LLVMContext &context;
    CompileHighLevel(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context);
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    std::map<std::string, llvm::Function *> get_map();
};

#endif