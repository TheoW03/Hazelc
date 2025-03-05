#include <backend/CompilerUtil.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"

CompilerContext::CompilerContext()
{
    /* here to make the compiler stop bitching */
}

CompilerContext::CompilerContext(std::map<std::string, llvm::Function *> CFunctions, std::map<TokenType, OptionalType> NativeTypes)
{

    this->CFunctions = CFunctions;
    this->NativeTypes = NativeTypes;
    this->string_type = nullptr;
}

Function CompilerContext::get_function(Tokens name)
{
    return func_map[name.value];
}

void CompilerContext::add_function(Tokens name, Function f)
{
    this->func_map.insert(std::make_pair(name.value, f));
}

llvm::StructType *CompilerContext::get_string_type(llvm::LLVMContext &context, llvm::IRBuilder<> &builder)
{
    if (this->string_type == nullptr)
    {

        this->string_type = llvm::StructType::create(context, "string");
        std::vector<llvm::Type *> elements = {builder.getInt8PtrTy(), builder.getInt64Ty()};
        this->string_type->setBody(elements);

        // uint64_t StructSize = DL.getTypeAllocSize(MyStruct);
    }
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
    exit(EXIT_FAILURE);
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
