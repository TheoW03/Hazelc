#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <Frontend/Ast.h>
#include "llvm/IR/LLVMContext.h"
#include <memory>
#include <backend/CompilerContext.h>
#include "llvm/IR/Intrinsics.h"
#include "CompilerUtil.h"

// these get the type of the expression
// used because the LLCM has seperation for floats and
TypeOfExpr get_bool_expr_type(std::shared_ptr<ASTNode> n, CompilerContext ctx)
{
    TypeOfExpr get_expr_type(std::shared_ptr<ASTNode> n, CompilerContext ctx);
    auto c = dynamic_cast<BooleanExprNode *>(n.get());
    if (dynamic_cast<IntegerNode *>(c->lhs.get()) && dynamic_cast<IntegerNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    else if (dynamic_cast<DecimalNode *>(c->lhs.get()) && dynamic_cast<DecimalNode *>(c->rhs.get()))
        return TypeOfExpr::Float_Type;
    else if (dynamic_cast<BooleanConstNode *>(c->lhs.get()) && dynamic_cast<BooleanConstNode *>(c->rhs.get()))
        return TypeOfExpr::Boolean_Type;
    else if (dynamic_cast<StringNode *>(c->lhs.get()) && dynamic_cast<StringNode *>(c->rhs.get()))
        return TypeOfExpr::String_Type;

    else if (dynamic_cast<NoneNode *>(c->lhs.get()) || dynamic_cast<NoneNode *>(c->rhs.get()))
        return TypeOfExpr::None_Type;
    if (dynamic_cast<FunctionCallNode *>(c->lhs.get()))
    {
        auto d = dynamic_cast<FunctionCallNode *>(c->lhs.get());
        auto f = ctx.get_function(d->hash_name.has_value() ? d->hash_name.value() : d->name.value).value();

        if (dynamic_cast<IntegerType *>(f.ret_type.get()) || dynamic_cast<ByteType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Integer_Type;
        }
        else if (dynamic_cast<DecimalType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Float_Type;
        }
        else if (dynamic_cast<BoolType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Boolean_Type;
        }
        else if (dynamic_cast<StringType *>(f.ret_type.get()))
        {
            return TypeOfExpr::String_Type;
        }
        // if (dynamic_cast<NativeType *>(f.ret_type.get()))
        // {
        //     auto p = dynamic_cast<NativeType *>(f.ret_type.get());
        //     if (p->type.type == TokenType::Integer)
        //     {
        //         return TypeOfExpr::Integer_Type;
        //     }
        //     else if (p->type.type == TokenType::Decimal)
        //     {
        //         return TypeOfExpr::Float_Type;
        //     }
        //     else if (p->type.type == TokenType::string)
        //     {
        //         return TypeOfExpr::String_Type;
        //     }
        //     else if (p->type.type == TokenType::boolean)
        //     {
        //         return TypeOfExpr::Boolean_Type;
        //     }
        // }
    }
    if (dynamic_cast<FunctionCallNode *>(c->rhs.get()))
    {
        auto d = dynamic_cast<FunctionCallNode *>(c->rhs.get());
        auto f = ctx.get_function(d->hash_name.has_value() ? d->hash_name.value() : d->name.value).value();
        if (dynamic_cast<IntegerType *>(f.ret_type.get()) || dynamic_cast<ByteType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Integer_Type;
        }
        else if (dynamic_cast<DecimalType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Float_Type;
        }
        else if (dynamic_cast<BoolType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Boolean_Type;
        }
        else if (dynamic_cast<StringType *>(f.ret_type.get()))
        {
            return TypeOfExpr::String_Type;
        }
    }
    if (dynamic_cast<BooleanExprNode *>(c->lhs.get()))
        return get_bool_expr_type(c->lhs, ctx);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return get_bool_expr_type(c->rhs, ctx);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return get_bool_expr_type(c->rhs, ctx);
    if (dynamic_cast<ExprNode *>(c->rhs.get()))
        return get_expr_type(c->rhs, ctx);
    if (dynamic_cast<ExprNode *>(c->lhs.get()))
        return get_expr_type(c->lhs, ctx);
    return TypeOfExpr::Void_Type;
}
TypeOfExpr get_expr_type(std::shared_ptr<ASTNode> n, CompilerContext ctx)
{
    auto c = dynamic_cast<ExprNode *>(n.get());
    if (dynamic_cast<IntegerNode *>(c->lhs.get()) && dynamic_cast<IntegerNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    else if (dynamic_cast<DecimalNode *>(c->lhs.get()) && dynamic_cast<DecimalNode *>(c->rhs.get()))
        return TypeOfExpr::Float_Type;
    else if (dynamic_cast<BooleanConstNode *>(c->lhs.get()) && dynamic_cast<BooleanConstNode *>(c->rhs.get()))
        return TypeOfExpr::Integer_Type;
    else if (dynamic_cast<StringNode *>(c->lhs.get()) && dynamic_cast<StringNode *>(c->rhs.get()))
        return TypeOfExpr::String_Type;
    if (dynamic_cast<FunctionCallNode *>(c->lhs.get()))
    {
        auto d = dynamic_cast<FunctionCallNode *>(c->lhs.get());
        auto f = ctx.get_function(d->hash_name.has_value() ? d->hash_name.value() : d->name.value).value();
        if (dynamic_cast<IntegerType *>(f.ret_type.get()) || dynamic_cast<ByteType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Integer_Type;
        }
        else if (dynamic_cast<DecimalType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Float_Type;
        }
        else if (dynamic_cast<BoolType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Boolean_Type;
        }
        else if (dynamic_cast<StringType *>(f.ret_type.get()))
        {
            return TypeOfExpr::String_Type;
        }
        // if (f.)
        // auto c =
    }
    if (dynamic_cast<FunctionCallNode *>(c->rhs.get()))
    {
        auto d = dynamic_cast<FunctionCallNode *>(c->rhs.get());
        auto f = ctx.get_function(d->hash_name.has_value() ? d->hash_name.value() : d->name.value).value();
        if (dynamic_cast<IntegerType *>(f.ret_type.get()) || dynamic_cast<ByteType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Integer_Type;
        }
        else if (dynamic_cast<DecimalType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Float_Type;
        }
        else if (dynamic_cast<BoolType *>(f.ret_type.get()))
        {
            return TypeOfExpr::Boolean_Type;
        }
        else if (dynamic_cast<StringType *>(f.ret_type.get()))
        {
            return TypeOfExpr::String_Type;
        }
        // if (f.)
        // auto c =
    }

    if (dynamic_cast<ExprNode *>(c->lhs.get()))
        return get_expr_type(c->lhs, ctx);
    if (dynamic_cast<ExprNode *>(c->rhs.get()))
        return get_expr_type(c->rhs, ctx);
    if (dynamic_cast<BooleanExprNode *>(c->rhs.get()))
        return TypeOfExpr::Boolean_Type;
    if (dynamic_cast<BooleanExprNode *>(c->lhs.get()))
        return TypeOfExpr::Boolean_Type;
    return TypeOfExpr::Void_Type;
}

std::optional<OptionalType> getTypeOfOnSide(std::shared_ptr<ASTNode> n, CompilerContext ctx)
{
    if (dynamic_cast<IntegerNode *>(n.get()))
        return ctx.get_integer_type();
    if (dynamic_cast<FunctionCallNode *>(n.get()))
    {
        auto d = dynamic_cast<FunctionCallNode *>(n.get());
        auto f = ctx.get_function(d->hash_name.has_value() ? d->hash_name.value() : d->name.value).value();

        if (dynamic_cast<IntegerType *>(f.ret_type.get()) || dynamic_cast<ByteType *>(f.ret_type.get()))
        {
            return ctx.get_integer_type();
        }
        else if (dynamic_cast<DecimalType *>(f.ret_type.get()))
        {
            return ctx.get_float_type();
        }
        else if (dynamic_cast<BoolType *>(f.ret_type.get()))
        {
            return ctx.get_boolean_type();
        }
        else if (dynamic_cast<StringType *>(f.ret_type.get()))
        {
            return ctx.get_string_type();
        }
    }
    return {};
}
// optional type. since in hazel all types are optional
// it compies as a structure
//  the actual valye
// and a Boolean to represent if its none or some
OptionalType::OptionalType()
{
}
OptionalType::OptionalType(llvm::LLVMContext &context, llvm::IRBuilder<> &builder, llvm::Type *inner)

{
    this->type = llvm::StructType::create(context, "OptionalType");
    this->inner = inner;
    std::vector<llvm::Type *> elements = {inner, builder.getInt1Ty()};
    this->type->setBody(elements);
}

llvm::Value *OptionalType::set_loaded_value(llvm::Value *value, llvm::IRBuilder<> &builder)
{

    llvm::Value *structPtr = builder.CreateAlloca(this->type);
    auto destField0ptr = builder.CreateStructGEP(this->type, structPtr, 0, "OptionalStructPtr0.Value");

    builder.CreateStore(value, destField0ptr);
    auto destField1ptr = builder.CreateStructGEP(this->type, structPtr, 1, "OptionalStructPtr1.isNone");
    auto isNone = llvm::ConstantInt::get(builder.getInt1Ty(), 0);
    builder.CreateStore(isNone, destField1ptr);
    return structPtr;
}

llvm::Value *OptionalType::set_loaded_struct_value(llvm::Function *memcpy, llvm::Value *value, llvm::IRBuilder<> &builder, size_t size)
{
    llvm::Value *structPtr = builder.CreateAlloca(this->type);
    auto destField0ptr = builder.CreateStructGEP(this->type, structPtr, 0, "OptionalStructPtr0.Value");
    builder.CreateCall(memcpy, {
                                   destField0ptr,
                                   value,
                                   llvm::ConstantInt::get(builder.getInt64Ty(), size),
                                   llvm::ConstantInt::get(builder.getInt1Ty(), 0),

                               });
    auto isNone = llvm::ConstantInt::get(builder.getInt1Ty(), 0);
    auto destField1ptr = builder.CreateStructGEP(this->type, structPtr, 1, "OptionalStructPtr1.isNone");
    builder.CreateStore(isNone, destField1ptr);
    return structPtr;
}

llvm::Value *OptionalType::get_value(llvm::IRBuilder<> &builder)
{
    return nullptr;
}

llvm::Value *OptionalType::get_none(llvm::IRBuilder<> &builder)
{
    llvm::Value *structPtr = builder.CreateAlloca(this->type);
    auto destField0ptr = builder.CreateStructGEP(this->type, structPtr, 0, "OptionalStructPtr0.Value");

    builder.CreateStore(llvm::Constant::getNullValue(inner), destField0ptr);
    auto destField1ptr = builder.CreateStructGEP(this->type, structPtr, 1, "OptionalStructPtr1.isNone");
    auto isNone = llvm::ConstantInt::get(builder.getInt1Ty(), 1);
    builder.CreateStore(isNone, destField1ptr);
    return structPtr;
    // llvm::Value *structPtr = builder.CreateAlloca(this->type);
    // auto destField1ptr = builder.CreateStructGEP(this->type, structPtr, 1, "OptionalStructPtr1");
    // auto isNone = llvm::ConstantInt::get(builder.getInt1Ty(), 1);
    // builder.CreateStore(isNone, destField1ptr);
}

llvm::Type *OptionalType::get_type()
{
    return this->type;
}
llvm::Value *OptionalType::get_inner_value(llvm::IRBuilder<> &builder, llvm::Value *value, bool load)
{
    auto v = builder.CreateStructGEP(this->type, value, 0, "getting_inner_value");
    if (load)
    {
        return builder.CreateLoad(inner, v, "get inner and load");
    }
    return v;
}

size_t OptionalType::get_type_size(llvm::Module &module)
{
    llvm::DataLayout datalayout(&module);
    return datalayout.getTypeAllocSize(this->type);
}

size_t OptionalType::get_inner_size(llvm::Module &module)
{
    llvm::DataLayout datalayout(&module);

    return datalayout.getTypeAllocSize(this->inner);
}

llvm::Value *ValueOrLoad(llvm::IRBuilder<> &builder, llvm::Value *value, llvm::Type *type)
{
    if (value->getType()->isPointerTy())
        value = builder.CreateLoad(type, value, "ValueOrLoad");
    return value;
}

Function::Function()
{
}

Function::Function(llvm::Function *function, std::vector<CompiledFunction> params, std::shared_ptr<Type> ret_type, Tokens name, bool isAnonymous)
{
    this->function = function;
    this->params = params;

    this->ret_type = ret_type;
    this->isAnonymous = isAnonymous;
}

CompiledFunction::CompiledFunction()
{
}

llvm::Value *Function::compile(CompilerContext ctx)
{
    return nullptr;
}

llvm::Type *Function::get_type()
{
    return this->function->getType();
}

CompiledPram::CompiledPram()
{
}

llvm::Value *CompiledPram::compile(CompilerContext ctx)
{
    return nullptr;
}
