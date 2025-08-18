#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <backend/CompilerUtil.h>

#ifndef FUNCTION_PARAM_H
#define FUNCTION_PARAM_H
struct FunctionParam
{

    Tokens name;
    llvm::StructType *thunk_type;
    llvm::FunctionType *type;
    std::shared_ptr<Type> ret_type;
    size_t gep_loc;
};
#endif

#ifndef COMPILED_FUNCTION_H
#define COMPILED_FUNCTION_H
struct Function
{
    llvm::Function *function;
    std::vector<Function> params;
    std::shared_ptr<Type> ret_type;
    Tokens name;
    std::vector<FunctionParam> thunks;
    bool isAnonymous;
};
#endif

#ifndef COMPILED_FUNCTION2_H
#define COMPILED_FUNCTION2_H
class CompilerContext;

class Compiled_Function
{
public:
    Compiled_Function();
    virtual std::shared_ptr<Type> get_ret_type() = 0;
    virtual ValueStruct compile(CompilerContext &ctx, llvm::BasicBlock *block, llvm::Module &module,
                                llvm::IRBuilder<> &builder,
                                llvm::LLVMContext &context) = 0;
};
#endif

#ifndef FUNCTION_H
#define FUNCTION_H
class CompilerContext;

class DefinedFunction : public Compiled_Function
{
public:
    Function function;
    DefinedFunction();
    DefinedFunction(Function function);
    std::shared_ptr<Type> get_ret_type() override;
    ValueStruct compile(CompilerContext &ctx, llvm::BasicBlock *block, llvm::Module &module,
                        llvm::IRBuilder<> &builder,
                        llvm::LLVMContext &context) override;
};
#endif

#ifndef Param_Function_H
#define Param_Function_H
class CompilerContext;

class ParamFunction : public Compiled_Function
{
public:
    FunctionParam thunk;
    ParamFunction();
    ParamFunction(FunctionParam thunk);
    std::shared_ptr<Type> get_ret_type() override;
    ValueStruct compile(CompilerContext &ctx, llvm::BasicBlock *block, llvm::Module &module,
                        llvm::IRBuilder<> &builder,
                        llvm::LLVMContext &context) override;
};
#endif