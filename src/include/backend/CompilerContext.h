
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <map>
#include <iostream>
#include <queue>
#include "llvm/IR/LLVMContext.h"
#include <backend/Scope.h>
#include <Frontend/Token.h>
#include <Frontend/Ast.h>

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

// #ifndef CONTEXT_H
// #define CONTEXT_H
class CompilerContext
{
public:
    std::map<std::string, Function> func_map;
    // std::map<std::string, llvm::Function *> CFunctions;
    std::map<TokenType, OptionalType> NativeTypes;
    Tokens current_module;
    CRunTimeFunctions CProcedures;
    // std::map<std::string, llvm::Type *> types;
    // std::map<TokenType, OptionalType> types;
    std::vector<llvm::StructType *> lists;
    llvm::StructType *string_type;
    CompilerContext();

    CompilerContext(CRunTimeFunctions CProcedures, llvm::LLVMContext &context, llvm::Module &module, llvm::IRBuilder<> &builder);
    // CompilerContext(
    //     CRunTimeFunctions CProcedures,
    //     std::map<TokenType, OptionalType> NativeTypes,
    //     llvm::StructType *str_type);
    // Function get_function(Tokens name);
    // Function get_local_function(Tokens name);
    // void add_local_function(Tokens name, Function function);
    // void add_function(Tokens name, Function f);
    // Function get_current();
    // void compile_cfunctions(llvm::Module &module, llvm::LLVMContext &context, llvm::IRBuilder<> &builder);
    llvm::StructType *get_string_inner_type();
    llvm::Type *compile_Type(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<Type> ty);
    // llvm::FunctionType *compile_Function_Type(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, llvm::StructType *params, std::shared_ptr<FunctionRefNode> n);

    OptionalType compile_Type_Optional(std::shared_ptr<Type> ty);
    // Thunks get_thunk_types(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<FunctionRefNode> n, llvm::StructType *params);

    OptionalType get_integer_type();
    OptionalType get_float_type();
    OptionalType get_string_type();
    OptionalType get_boolean_type();
    OptionalType get_byte_type();
    OptionalType get_type(std::shared_ptr<Type> type);

    // void AddModule(std::string module_name, CompiledModule module);
    // ProgramScope getScope();
    // CompiledModule get_module(Tokens module);
    // CompiledModule get_current_module();
    // void set_current_module(Tokens module_name);
    // bool can_get_function(Tokens name);
};
#endif

TypeOfExpr get_expr_type(std::shared_ptr<ASTNode> n, ProgramScope ctx);
TypeOfExpr get_bool_expr_type(std::shared_ptr<ASTNode> n, ProgramScope ctx);