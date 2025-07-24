
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <map>
#include <iostream>
#include <queue>
#include "llvm/IR/LLVMContext.h"
#include <Frontend/Token.h>
#include <Frontend/Ast.h>
#include <backend/CompilerUtil.h>
// #include "compiler_visitors.h"

#ifndef OPTIONAL_TYPE_H
#define OPTIONAL_TYPE_H
class OptionalType
{
public:
    llvm::StructType *type;
    llvm::Type *inner;
    OptionalType();
    OptionalType(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Type *inner);
    llvm::Value *set_loaded_value(llvm::Value *value, llvm::IRBuilder<> &builder);
    llvm::Value *set_loaded_struct_value(llvm::Function *memcpy, llvm::Value *value, llvm::IRBuilder<> &builder, size_t size);
    llvm::Value *get_value(llvm::IRBuilder<> &builder);
    llvm::Value *get_none(llvm::IRBuilder<> &builder);
    llvm::Type *get_type();

    llvm::Value *get_inner_value(llvm::IRBuilder<> &builder, llvm::Value *value, bool load);
    size_t get_type_size(llvm::Module &module);
    size_t get_inner_size(llvm::Module &module);
};
#endif

#ifndef CONTEXT_H
#define CONTEXT_H
class Compiled_Function;
class CompilerContext
{
public:
    std::map<std::string, std::shared_ptr<Compiled_Function>> global_functions;
    std::map<std::string, std::shared_ptr<Compiled_Function>> local_functions;
    llvm::StructType *params;

    std::stack<Function> functions;
    std::map<TokenType, OptionalType> NativeTypes;
    CRunTimeFunctions CProcedures;
    Function current_function;
    std::vector<llvm::StructType *> lists;
    llvm::StructType *string_type;
    CompilerContext();

    CompilerContext(CRunTimeFunctions CProcedures, llvm::LLVMContext &context, llvm::Module &module, llvm::IRBuilder<> &builder);
    llvm::StructType *get_string_inner_type();
    llvm::Type *compile_Type(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<Type> ty);
    OptionalType compile_Type_Optional(std::shared_ptr<Type> ty);
    OptionalType get_integer_type();
    OptionalType get_float_type();
    OptionalType get_string_type();
    OptionalType get_boolean_type();
    OptionalType get_byte_type();
    OptionalType get_type(std::shared_ptr<Type> type);
    void add_function(FunctionNode *node, Function function);
    std::optional<std::shared_ptr<Compiled_Function>> get_function(FunctionCallNode *node);
    std::optional<int> addLocal(Tokens name, std::shared_ptr<Compiled_Function> function);
    Function get_current_function();
    Function set_current_function();
    void set_params(llvm::StructType *params);
};
#endif

#ifndef COMPILED_FUNCTION2_H
#define COMPILED_FUNCTION2_H

class Compiled_Function
{
public:
    Compiled_Function();
    virtual std::shared_ptr<Type> get_ret_type() = 0;
    virtual ValueStruct compile(CompilerContext ctx, llvm::BasicBlock *block, llvm::Module &module,
                                llvm::IRBuilder<> &builder,
                                llvm::LLVMContext &context) = 0;
};
#endif

#ifndef FUNCTION_H
#define FUNCTION_H

class DefinedFunction : public Compiled_Function
{
public:
    Function function;
    DefinedFunction();
    DefinedFunction(Function function);
    std::shared_ptr<Type> get_ret_type() override;
    ValueStruct compile(CompilerContext ctx, llvm::BasicBlock *block, llvm::Module &module,
                        llvm::IRBuilder<> &builder,
                        llvm::LLVMContext &context) override;
};
#endif

#ifndef Param_Function_H
#define Param_Function_H

class ParamFunction : public Compiled_Function
{
public:
    Thunks thunk;
    ParamFunction();
    ParamFunction(Thunks thunk);
    std::shared_ptr<Type> get_ret_type() override;
    ValueStruct compile(CompilerContext ctx, llvm::BasicBlock *block, llvm::Module &module,
                        llvm::IRBuilder<> &builder,
                        llvm::LLVMContext &context) override;
};
#endif