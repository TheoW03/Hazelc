#include <backend/CompilerUtil.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include <backend/CompilerContext.h>
#include <backend/Compiled_Functions.h>

CompilerContext::CompilerContext()
{
}
CompilerContext::CompilerContext(CRunTimeFunctions CProcedures, llvm::LLVMContext &context, llvm::Module &module, llvm::IRBuilder<> &builder)
{
    auto string_type = llvm::StructType::create(context, "string");
    std::vector<llvm::Type *> elements = {builder.getPtrTy(), builder.getInt64Ty()};
    string_type->setBody(elements);
    this->string_type = string_type;
    NativeTypes.insert(std::make_pair(TokenType::Integer, OptionalType(context, builder, builder.getInt64Ty())));
    NativeTypes.insert(std::make_pair(TokenType::Uinteger, OptionalType(context, builder, builder.getInt64Ty())));

    NativeTypes.insert(std::make_pair(TokenType::Decimal, OptionalType(context, builder, builder.getDoubleTy())));
    NativeTypes.insert(std::make_pair(TokenType::boolean, OptionalType(context, builder, builder.getInt1Ty())));

    NativeTypes.insert(std::make_pair(TokenType::character, OptionalType(context, builder, builder.getInt8Ty())));
    NativeTypes.insert(std::make_pair(TokenType::Byte, OptionalType(context, builder, builder.getInt8Ty())));
    NativeTypes.insert(std::make_pair(TokenType::Ubyte, OptionalType(context, builder, builder.getInt8Ty())));

    NativeTypes.insert(std::make_pair(TokenType::string, OptionalType(context, builder, string_type)));
    this->CProcedures = CProcedures;
}

llvm::StructType *CompilerContext::get_string_inner_type()
{
    return this->string_type;
}
llvm::Type *CompilerContext::compile_Type(llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<Type> ty)
{

    //     // TODO:
    //     // very temporary solution. need to fix later to include functions

    if (dynamic_cast<IntegerType *>(ty.get()))
    {
        return builder.getInt64Ty();
    }
    else if (dynamic_cast<ByteType *>(ty.get()))
    {
        return builder.getInt8Ty();
    }
    else if (dynamic_cast<BoolType *>(ty.get()))
    {
        return builder.getInt1Ty();
    }
    else if (dynamic_cast<StringType *>(ty.get()))
    {
        return this->get_string_inner_type();
    }
    else if (dynamic_cast<DecimalType *>(ty.get()))
    {
        return builder.getDoubleTy();
    }
    else if (dynamic_cast<CharacterType *>(ty.get()))
    {
        return builder.getInt8Ty();
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

    if (dynamic_cast<IntegerType *>(ty.get()))
    {
        return NativeTypes[TokenType::Integer];
    }
    else if (dynamic_cast<ByteType *>(ty.get()))
    {
        return NativeTypes[TokenType::Byte];
    }
    else if (dynamic_cast<BoolType *>(ty.get()))
    {
        return NativeTypes[TokenType::boolean];
    }
    else if (dynamic_cast<StringType *>(ty.get()))
    {
        return NativeTypes[TokenType::string];
    }
    else if (dynamic_cast<DecimalType *>(ty.get()))
    {
        return NativeTypes[TokenType::Decimal];
    }
    else if (dynamic_cast<CharacterType *>(ty.get()))
    {
        return NativeTypes[TokenType::character];
    }

    else
    {
        std::cout << "error" << std::endl;
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
    return compile_Type_Optional(type);
}

void CompilerContext::add_function(FunctionNode *node, Function function)
{
    if (node->hash_name.has_value())
    {
        this->global_functions.insert(std::make_pair(node->hash_name.value(), std::make_shared<DefinedFunction>(function)));
    }
    this->functions.push(function);
}

std::optional<std::shared_ptr<Compiled_Function>> CompilerContext::get_function(FunctionCallNode *node)
{
    if (node->hash_name.has_value() && this->global_functions.find(node->hash_name.value()) != this->global_functions.end())
    {
        return this->global_functions[node->hash_name.value()];
    }
    if (this->local_functions.find(node->name.value) != this->local_functions.end())
    {
        return this->local_functions[node->name.value];
    }
    return {};
}

std::optional<int> CompilerContext::addLocal(Tokens name, std::shared_ptr<Compiled_Function> function)
{
    if (this->local_functions.find(name.value) != this->local_functions.end())
    {
        local_functions[name.value] = function;
        return {};
    }
    else
    {
        local_functions.insert(std::make_pair(name.value, function));
        return 1;
    }
}
Function CompilerContext::get_current_function()

{
    return this->current_function;
}

Function CompilerContext::set_current_function()
{
    this->current_function = this->functions.top();
    this->functions.pop();
    return this->current_function;
}

void CompilerContext::set_params(llvm::StructType *params)
{
    this->params = params;
}
