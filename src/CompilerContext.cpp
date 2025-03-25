#include <backend/CompilerUtil.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

CompilerContext::CompilerContext()
{
    /* here to make the compiler stop bitching */
}

CompilerContext::CompilerContext(std::map<std::string, llvm::Function *> CFunctions,
                                 std::map<TokenType, OptionalType> NativeTypes, llvm::StructType *str_type)
{

    this->CFunctions = CFunctions;
    this->NativeTypes = NativeTypes;

    this->string_type = str_type;
}

Function CompilerContext::get_function(Tokens name)
{
    if (can_get_function(name))
    {
        if (modules[current_module.value].func_map.find(name.value) == modules[current_module.value].func_map.end())
        {
            auto import_list = modules[current_module.value].imports;
            for (int i = 0; i < import_list.size(); i++)
            {
                if (modules[import_list[i].value].func_map.find(name.value) != modules[current_module.value].func_map.end())
                {
                    return modules[import_list[i].value].func_map[name.value];
                }
            }
        }
        return modules[current_module.value].func_map[name.value];
    }
    else
    {
        return get_local_function(name);
    }
}

Function CompilerContext::get_local_function(Tokens name)
{
    return local_functions[name.value];
}
void CompilerContext::add_local_function(Tokens name, Function value)
{
    if (this->local_functions.find(name.value) != this->local_functions.end())
    {
        local_functions[name.value] = value;
    }
    else
    {
        local_functions.insert(std::make_pair(name.value, value));
    }
}
void CompilerContext::add_function(Tokens name, Function f)
{

    this->func_map.insert(std::make_pair(name.value, f));
}

Function CompilerContext::get_current()
{

    auto f = this->modules[current_module.value].functions.front();
    this->modules[current_module.value].functions.pop();
    return f;
}
llvm::StructType *CompilerContext::get_string_type(llvm::LLVMContext &context, llvm::IRBuilder<> &builder)
{
    return string_type;
}
llvm::Type *CompilerContext::compile_Type(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<Type> ty)
{

    //     // TODO:
    //     // very temporary solution. need to fix later to include functions
    if (dynamic_cast<NativeType *>(ty.get()))
    {
        auto p = dynamic_cast<NativeType *>(ty.get());
        if (p->type.type == TokenType::Integer || p->type.type == TokenType::Uinteger)
        {
            return builder.getInt64Ty();
        }
        else if (p->type.type == TokenType::Decimal)
        {
            return builder.getDoubleTy();
        }
        else if (p->type.type == TokenType::Byte || p->type.type == TokenType::character || p->type.type == TokenType::Ubyte)
        {
            return builder.getInt8Ty();
        }
        else if (p->type.type == TokenType::boolean)
        {
            return builder.getInt1Ty();
        }
        else if (p->type.type == TokenType::string)
        {
            return this->get_string_type(context, builder);
        }
    }
    // NOTE:
    //  for lists generate the entire list and when index simply shrink down the list
    //  and return the value indexed
    else if (dynamic_cast<ListType *>(ty.get()))
    {
        auto p = dynamic_cast<ListType *>(ty.get());

        auto c = this->compile_Type(builder, context, p->inner_type);
        for (int i = 0; i < this->lists.size(); i++)
        {
            if (this->lists[i]->getElementType(0) == c)
            {
                return llvm::PointerType::getUnqual(this->lists[i]);
            }
        }
        llvm::StructType *nodeType = llvm::StructType::create(context, "Node");
        std::vector<llvm::Type *> elements = {c, llvm::PointerType::getUnqual(nodeType)};
        nodeType->setBody(elements);

        this->lists.push_back((nodeType));
        return llvm::PointerType::getUnqual(nodeType);
    }
    return builder.getVoidTy();
}

llvm::FunctionType *CompilerContext::compile_Function_Type(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<FunctionRefNode> n)
{
    auto c = n->RetType;
    std::vector<llvm::Type *> a;
    for (int i = 0; i < n->params.size(); i++)
    {
        auto funct = compile_Function_Type(builder, context, n->params[i]);
        a.push_back(get_thunk_types(builder, context, n->params[i]).thunk_type);
    }
    llvm::FunctionType *functype = llvm::FunctionType::get(
        compile_Type_Optional(c).type, a, false);
    return functype;
}
OptionalType CompilerContext::compile_Type_Optional(std::shared_ptr<Type> ty)
{
    if (dynamic_cast<NativeType *>(ty.get()))
    {
        auto p = dynamic_cast<NativeType *>(ty.get());
        return NativeTypes[p->type.type];
    }
}
Thunks CompilerContext::get_thunk_types(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<FunctionRefNode> n)

{

    // std::vector<Thunks> thunks;
    // this->string_type = llvm::StructType::create(context, "Thunk");
    auto thunk = llvm::StructType::create(context, "Thunk");
    auto funct = compile_Function_Type(builder, context, n);
    std::vector<llvm::Type *> elements = {compile_Type_Optional(n->RetType).type, llvm::PointerType::getUnqual(funct), builder.getInt1Ty()};
    // this->string_type->setBody(elements);
    thunk->setBody(elements);
    return {thunk, nullptr};
}

OptionalType CompilerContext::get_integer_type()
{
    return NativeTypes[TokenType::Integer];
}

OptionalType CompilerContext::get_float_type()
{
    return NativeTypes[TokenType::Decimal];
}

OptionalType CompilerContext::get_string_type()
{

    return NativeTypes[TokenType::string];
}

OptionalType CompilerContext::get_boolean_type()
{
    return NativeTypes[TokenType::boolean];
}

OptionalType CompilerContext::get_byte_type()
{
    return NativeTypes[TokenType::character];
}

OptionalType CompilerContext::get_type(std::shared_ptr<Type> type)
{
    if (dynamic_cast<NativeType *>(type.get()))
    {
        auto p = dynamic_cast<NativeType *>(type.get());
        if (p->type.type == TokenType::Integer)
        {
            return get_integer_type();
        }
        else if (p->type.type == TokenType::Decimal)
        {
            return get_float_type();
        }
        else if (p->type.type == TokenType::string)
        {
            return get_string_type();
        }
        else if (p->type.type == TokenType::character || p->type.type == TokenType::Byte)
        {
            return get_byte_type();
        }
    }
}

void CompilerContext::AddModule(std::string module_name, CompiledModule module)
{
    modules.insert(std::make_pair(module_name, module));
}

CompiledModule CompilerContext::get_module(Tokens module)
{
    return this->modules[module.value];
}

CompiledModule CompilerContext::get_current_module()
{
    return this->modules[current_module.value];
}
void CompilerContext::set_current_module(Tokens module_name)
{
    this->current_module = module_name;
}

bool CompilerContext::can_get_function(Tokens name)
{
    if (modules[current_module.value].func_map.find(name.value) == modules[current_module.value].func_map.end())
    {
        auto import_list = modules[current_module.value].imports;
        for (int i = 0; i < import_list.size(); i++)
        {
            if (modules[import_list[i].value].func_map.find(name.value) != modules[import_list[i].value].func_map.end())
            {
                return true;
            }
        }
        return false;
    }
    return true;
}
