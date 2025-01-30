#include <visitor.h>
#include <backend/CompilerUtil.h>

CompileExpr::CompileExpr(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, CompilerContext &compiler_context) : module(module), builder(builder), context(context), compiler_context(compiler_context)
{
    // this->func_map = func_map;
}

llvm::Value *CompileExpr::IntMathExpression(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    switch (op.type)
    {
    case Addition:
        return builder.CreateAdd(lhs, rhs, "addition");
    case Multiplication:
        return builder.CreateMul(lhs, rhs, "addition");
    case Division:
        return builder.CreateSDiv(lhs, rhs, "addition");
    case Subtraction:
        return builder.CreateSub(lhs, rhs, "addition");
    case Modulas:
        return builder.CreateSRem(lhs, rhs, "modulas");
    case And:
        return builder.CreateAnd(lhs, rhs, "modulas");
    case Or:
        return builder.CreateOr(lhs, rhs, "modulas");
    case Left_Shift:
        return builder.CreateShl(lhs, rhs, "leftshoit");
    case Right_Shift:
        return builder.CreateLShr(lhs, rhs, "right_shift");
    default:
        std::cout << "semantic anaylsis bug perhaps" << std::endl;
        exit(EXIT_FAILURE);
    }
    return nullptr;
}

llvm::Value *CompileExpr::FloatMathExpression(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    switch (op.type)
    {
    case Addition:
        return builder.CreateFAdd(lhs, rhs, "addition");
    case Multiplication:
        return builder.CreateFMul(lhs, rhs, "addition");
    case Division:
        return builder.CreateFDiv(lhs, rhs, "addition");
    case Subtraction:
        return builder.CreateFSub(lhs, rhs, "addition");
    default:
        std::cout << "semantic anaylsis bug perhaps" << std::endl;
        exit(EXIT_FAILURE);
    }
    return nullptr;
}

llvm::Value *CompileExpr::BoolIntMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    switch (op.type)
    {
    case EQ:
        return builder.CreateICmp(llvm::CmpInst::ICMP_EQ, lhs, rhs, "eq");
    case LT:
        return builder.CreateICmp(llvm::CmpInst::ICMP_SLT, lhs, rhs, "LT");
    case LTE:
        return builder.CreateICmp(llvm::CmpInst::ICMP_SLE, lhs, rhs, "LE");
    case GT:
        return builder.CreateICmp(llvm::CmpInst::ICMP_SGT, lhs, rhs, "GT");
    case GTE:
        return builder.CreateICmp(llvm::CmpInst::ICMP_SGE, lhs, rhs, "GE");
    case NE:
        return builder.CreateICmp(llvm::CmpInst::ICMP_NE, lhs, rhs, "NE");
    default:
        std::cout << "semantic anaylsis bug perhaps in boolean \"" << op.value << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
}

llvm::Value *CompileExpr::BoolFloatMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    return nullptr;
}

llvm::Value *CompileExpr::Expression(std::shared_ptr<ASTNode> node)
{
    if (dynamic_cast<IntegerNode *>(node.get()))
    {
        auto c = dynamic_cast<IntegerNode *>(node.get());
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), c->number);
    }
    else if (dynamic_cast<DecimalNode *>(node.get()))
    {
        auto c = dynamic_cast<DecimalNode *>(node.get());
        return llvm::ConstantFP::get(context, llvm::APFloat(c->number));
    }
    else if (dynamic_cast<BooleanConstNode *>(node.get()))
    {
        auto c = dynamic_cast<BooleanConstNode *>(node.get());
        return llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), c->value.type == TokenType::True ? 1 : 0);
    }
    else if (dynamic_cast<StringNode *>(node.get()))
    {
        auto c = dynamic_cast<StringNode *>(node.get());

        auto a = compiler_context.get_string_type(context, builder);
        llvm::Value *structPtr = builder.CreateAlloca(a);
        auto str = builder.CreateGlobalString(c->value.value);
        auto Field0Ptr = builder.CreateStructGEP(a, structPtr, 0, "field0Ptr");
        builder.CreateStore(Field0Ptr, str);
        auto Field1Ptr = builder.CreateStructGEP(a, structPtr, 1, "field1Ptr");
        builder.CreateStore(Field1Ptr, llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), c->value.value.size()));
        return builder.CreateLoad(a, structPtr);
    }
    else if (dynamic_cast<ExprNode *>(node.get()))
    {
        auto c = dynamic_cast<ExprNode *>(node.get());

        auto lhs = Expression(c->lhs);
        auto rhs = Expression(c->rhs);
        auto get_type = get_expr_type(node);
        switch (get_type)
        {
        case Integer_Type:
            return IntMathExpression(lhs, c->operation, rhs);
        case Float_Type:
            return IntMathExpression(lhs, c->operation, rhs);
        default:
            break;
        }
    }
    else if (dynamic_cast<BooleanExprNode *>(node.get()))
    {
        auto c = dynamic_cast<BooleanExprNode *>(node.get());

        auto lhs = Expression(c->lhs);
        auto rhs = Expression(c->rhs);
        auto get_type = get_bool_expr_type(node);
        switch (get_type)
        {
        case Integer_Type:
            return BoolIntMathExpr(lhs, c->op, rhs);
        case Float_Type:
            return BoolFloatMathExpr(lhs, c->op, rhs);
        default:
            break;
        }
    }
    return nullptr;
}
