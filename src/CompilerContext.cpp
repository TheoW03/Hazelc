#include <backend/CompilerUtil.h>
#include "llvm/IR/Module.h"
#include <backend/Scope.h>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include <backend/CompilerContext.h>
// #include <P.h>

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

llvm::StructType *CompilerContext::get_string_inner_type()
{
    return this->string_type;
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
            return this->get_string_inner_type();
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

OptionalType CompilerContext::compile_Type_Optional(std::shared_ptr<Type> ty)
{
    if (dynamic_cast<NativeType *>(ty.get()))
    {
        auto p = dynamic_cast<NativeType *>(ty.get());
        return NativeTypes[p->type.type];
    }
    else
    {
        exit(EXIT_FAILURE);
    }
    // return
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
        if (p->type.type == TokenType::Integer || p->type.type == TokenType::Uinteger)
        {
            return get_integer_type();
        }
        else if (p->type.type == TokenType::boolean)
        {
            return get_boolean_type();
        }
        else if (p->type.type == TokenType::Decimal)
        {
            return get_float_type();
        }
        else if (p->type.type == TokenType::string)
        {
            return get_string_type();
        }
        else if (p->type.type == TokenType::character || p->type.type == TokenType::Byte || p->type.type == TokenType::Ubyte)
        {
            return get_byte_type();
        }
    }
    exit(EXIT_FAILURE);
}

void CompilerContext::AddModule(std::string module_name, CompiledModule module)
{

    modules.insert(std::make_pair(module_name, CompiledModuleClass(module)));
}

ProgramScope CompilerContext::getScope()
{
    return ProgramScope(modules);
}
