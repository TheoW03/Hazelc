#include <visitor.h>
#include <backend/CompilerUtil.h>

CompileExpr::CompileExpr(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::map<std::string, Function> func_map) : module(module), builder(builder), context(context)
{
    this->func_map = func_map;
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
    return nullptr;
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
    else if (dynamic_cast<ExprNode *>(node.get()))
    {
        auto c = dynamic_cast<ExprNode *>(node.get());

        auto lhs = Expression(c->lhs);
        auto rhs = Expression(c->rhs);
        // TODO:
        // very temporary solution. need to fix later to include functions
        if (dynamic_cast<IntegerNode *>(c->lhs.get()) && dynamic_cast<IntegerNode *>(c->rhs.get()))
            return IntMathExpression(lhs, c->operation, rhs);
        else if (dynamic_cast<DecimalNode *>(c->lhs.get()) && dynamic_cast<DecimalNode *>(c->rhs.get()))
            return FloatMathExpression(lhs, c->operation, rhs);
    }
    return nullptr;
}
