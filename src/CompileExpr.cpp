#include <backend/compiler_visitors.h>
#include <backend/CompilerUtil.h>
#include <backend/CompilerContext.h>

CompileExpr::CompileExpr(llvm::Module &module,
                         llvm::IRBuilder<> &builder,
                         llvm::LLVMContext &context,
                         CompilerContext compiler_context,
                         ProgramScope program,
                         llvm::BasicBlock *block) : module(module),
                                                    builder(builder),
                                                    context(context)
{
    this->compiler_context = compiler_context;
    this->program = program;
    this->block = block;

    // this->func_map = func_map;
}
llvm::Value *CompileExpr::CompileStr(llvm::Value *str, llvm::Value *length, llvm::Value *structure)
{

    // strings are strcutures
    auto c = compiler_context.get_string_type(this->context, builder);
    auto destField0ptr = builder.CreateStructGEP(c, structure, 0, "destStructPtrF0");
    builder.CreateStore(str, destField0ptr);
    auto destField1ptr = builder.CreateStructGEP(c, structure, 1, "destStructPtrF1");
    builder.CreateStore(length, destField1ptr);
    return structure;
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
llvm::Value *CompileExpr::IntegerMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    auto integer_type = compiler_context.get_integer_type();
    auto lhs_val = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(integer_type.type, lhs, 0, "int_lhs"));
    auto rhs_val = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(integer_type.type, rhs, 0, "int_rhs"));
    auto math = IntMathExpression(lhs_val, op, rhs_val);
    return integer_type.set_loaded_value(math, builder);
}
llvm::Value *CompileExpr::FloatMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{

    auto float_type = compiler_context.get_float_type();
    auto lhs_val = builder.CreateLoad(builder.getDoubleTy(), builder.CreateStructGEP(float_type.type, lhs, 0, "int_lhs"));
    auto rhs_val = builder.CreateLoad(builder.getDoubleTy(), builder.CreateStructGEP(float_type.type, rhs, 0, "int_rhs"));
    auto math = FloatMathExpression(lhs_val, op, rhs_val);
    return float_type.set_loaded_value(math, builder);
}
llvm::Value *CompileExpr::StringMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{

    auto string_type = compiler_context.get_string_type();
    auto inner = compiler_context.get_string_type(context, builder);

    auto lhs_val = builder.CreateLoad(inner, builder.CreateStructGEP(string_type.type, lhs, 0, "str_lhs"));
    auto rhs_val = builder.CreateLoad(inner, builder.CreateStructGEP(string_type.type, rhs, 0, "str_rhs"));
    // rhs_val->getType()->();
    llvm::Type *float_type = rhs_val->getType();
    llvm::PointerType *ptrType = llvm::PointerType::get(rhs_val->getType(), 0);

    // wierd. to access a string, or ptr it must be bit cased 0.0
    // hate that
    auto rhs_struct_ptr = builder.CreateBitCast(rhs_val, ptrType);
    auto lhs_struct_ptr = builder.CreateBitCast(lhs_val, ptrType);

    auto math = StringMathExpr(lhs_struct_ptr, op, rhs_struct_ptr);
    // builder.CreateCall(compiler_context.CFunctions["printf"], {builder.CreateGlobalString("[HAZELC DEBUG]: %s \n"),
    //                                                            builder.CreateLoad(builder.getInt8PtrTy(),
    //                                                                               builder.CreateStructGEP(inner, math, 0, "destStructPtrF0"))});s

    return string_type.set_loaded_value(math, builder);
}
llvm::Value *CompileExpr::IntegerBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    auto integer_type = compiler_context.get_integer_type();
    auto BoolType = compiler_context.get_boolean_type();
    auto lhs_val = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(integer_type.type, lhs, 0, "int_lhs"));
    auto rhs_val = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(integer_type.type, rhs, 0, "int_rhs"));
    auto math = BoolIntMathExpr(lhs_val, op, rhs_val);
    return BoolType.set_loaded_value(math, builder);
}

llvm::Value *CompileExpr::FloatBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{

    auto float_type = compiler_context.get_float_type();
    auto BoolType = compiler_context.get_boolean_type();
    auto lhs_val = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(float_type.type, lhs, 0, "int_lhs"));
    auto rhs_val = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(float_type.type, rhs, 0, "int_rhs"));
    auto math = BoolFloatMathExpr(lhs_val, op, rhs_val);
    return BoolType.set_loaded_value(math, builder);
}
ValueStruct CompileExpr::CompileBranch(std::vector<std::shared_ptr<ASTNode>> stmnts)
{
    for (int i = 0; i < stmnts.size(); i++)
    {
        if (dynamic_cast<ReturnNode *>(stmnts[i].get()))
        {
            auto c = dynamic_cast<ReturnNode *>(stmnts[i].get());
            return Expression(c->Expr);
        }
    }
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

    switch (op.type)
    {
    case EQ:
        return builder.CreateFCmp(llvm::CmpInst::FCMP_OEQ, lhs, rhs, "eq");
    case LT:
        return builder.CreateFCmp(llvm::CmpInst::FCMP_OLT, lhs, rhs, "LT");
    case LTE:
        return builder.CreateFCmp(llvm::CmpInst::FCMP_OLE, lhs, rhs, "LE");
    case GT:
        return builder.CreateFCmp(llvm::CmpInst::FCMP_OGT, lhs, rhs, "GT");
    case GTE:
        return builder.CreateFCmp(llvm::CmpInst::FCMP_OGE, lhs, rhs, "GE");
    case NE:
        return builder.CreateFCmp(llvm::CmpInst::FCMP_ONE, lhs, rhs, "NE");
    default:
        std::cout << "hazelc: semantic analysis bug perhaps in boolean \"" << op.value << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
}

llvm::Value *CompileExpr::StringMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{

    switch (op.type)
    {
    case Concation:
    {
        auto fmt = builder.CreateGlobalString("%s%s");
        auto snprinft = compiler_context.CFunctions["snprintf"];
        // auto c = comiler_context.get_string_type(context, builder);
        auto c = compiler_context.get_string_type(context, builder);

        auto lenthlhs = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(c, lhs, 1, "str1"));
        auto lenthrhs = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(c, rhs, 1, "str2"));
        auto added_lengths = builder.CreateAdd(lenthlhs, lenthrhs);
        //    builder.CreateStructGEP(c, rhs, 1, "str1")
        auto dest = builder.CreateAlloca(builder.getInt8PtrTy(), added_lengths);
        auto strRhsPtr = builder.CreateLoad(builder.getInt8PtrTy(), builder.CreateStructGEP(c, rhs, 0, "str1"));
        auto strLhsPtr = builder.CreateLoad(builder.getInt8PtrTy(), builder.CreateStructGEP(c, lhs, 0, "str1"));

        builder.CreateCall(snprinft, {
                                         dest,
                                         added_lengths,
                                         fmt,
                                         strLhsPtr,
                                         strRhsPtr,

                                     });
        llvm::Value *destStructPtr = builder.CreateAlloca(c);

        return this->CompileStr(dest, added_lengths, destStructPtr);

        // return lhs;
    }
    default:
    {
        std::cout << "hazelc: semantic analysis bug perhaps in string operators \"" << op.value << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
    }
    // DEBUG STRCAT:

    // return destStructPtr;
}

ValueStruct CompileExpr::Expression(std::shared_ptr<ASTNode> node)
{
    if (dynamic_cast<IntegerNode *>(node.get()))
    {

        auto c = dynamic_cast<IntegerNode *>(node.get());
        auto get_int_type = compiler_context.get_integer_type();
        auto number = llvm::ConstantInt::get(builder.getInt64Ty(), c->number);

        return {this->block, get_int_type.set_loaded_value(number, builder)};
    }
    else if (dynamic_cast<BranchNode *>(node.get()))
    {
        // auto =
        // return Expression()
        auto branch = dynamic_cast<BranchNode *>(node.get());

        // auto c = dynamic_cast<ReturnNode *>(branch->stmnts[0].get());
        return Expression(branch->condition);
        // return Expression()
    }
    else if (dynamic_cast<NoneNode *>(node.get()))
    {
        return {
            this->block, compiler_context.get_type(program.get_current_function().ret_type).get_none(builder)};
    }
    else if (dynamic_cast<ConditionalNode *>(node.get()))
    {
        auto condition_stmnt = dynamic_cast<ConditionalNode *>(node.get());
        // program.get_current_function().function.
        // llvm::BasicBlock *ifTrue = llvm::BasicBlock::Create(context, "if.true", program.get_current_function().function);
        auto type = compiler_context.get_type(condition_stmnt->type);
        std::vector<std::tuple<llvm::BasicBlock *, llvm::Value *>> phi_nodes;
        llvm::BasicBlock *endTrue = llvm::BasicBlock::Create(context, "end.true", program.get_current_function().function);
        for (int i = 0; i < condition_stmnt->branches.size(); i++)
        {
            auto condition = Expression(condition_stmnt->branches[i]->condition).value;
            llvm::BasicBlock *ifTrue = llvm::BasicBlock::Create(context, "if.true", program.get_current_function().function);
            condition = builder.CreateLoad(builder.getInt1Ty(),
                                           builder.CreateStructGEP(compiler_context.get_boolean_type().type, condition, 0, "str2"));

            if (i < condition_stmnt->branches.size() - 1)
            {
                llvm::BasicBlock *ElsTrue = llvm::BasicBlock::Create(context, "else.true", program.get_current_function().function);
                builder.CreateCondBr(condition, ifTrue, ElsTrue);
                builder.SetInsertPoint(ifTrue);
                this->block = ifTrue;
                auto value = CompileBranch(condition_stmnt->branches[i]->stmnts);
                auto loaded_val = ValueOrLoad(builder, value.value, type.get_type());
                phi_nodes.push_back({value.block, loaded_val});
                builder.CreateBr(endTrue);
                builder.SetInsertPoint(ElsTrue);
            }
            else
            {
                // builder.CreateCondBr(condition, ifTrue, endTrue);
                builder.CreateBr(ifTrue);
                builder.SetInsertPoint(ifTrue);
                this->block = ifTrue;
                auto value = CompileBranch(condition_stmnt->branches[i]->stmnts);
                auto loaded_val = ValueOrLoad(builder, value.value, type.get_type());
                phi_nodes.push_back({value.block, loaded_val});
                builder.CreateBr(endTrue);
            }
        }
        builder.SetInsertPoint(endTrue);
        this->block = endTrue;
        llvm::PHINode *phi = builder.CreatePHI(type.type, phi_nodes.size(), "iftmp");
        for (int i = 0; i < phi_nodes.size(); i++)
        {
            phi->addIncoming(std::get<1>(phi_nodes[i]), std::get<0>(phi_nodes[i]));
        }
        return {endTrue, phi};
    }
    else if (dynamic_cast<CharNode *>(node.get()))
    {

        auto c = dynamic_cast<CharNode *>(node.get());
        auto char_type = compiler_context.get_byte_type();
        auto value = llvm::ConstantInt::get(llvm::Type::getInt8Ty(context), c->value.value[0]);

        return {this->block, char_type.set_loaded_value(value, builder)};
    }
    else if (dynamic_cast<DecimalNode *>(node.get()))
    {
        auto c = dynamic_cast<DecimalNode *>(node.get());
        auto float_type = compiler_context.get_float_type();
        auto value = llvm::ConstantFP::get(context, llvm::APFloat(c->number));
        return {this->block, float_type.set_loaded_value(value, builder)};
    }
    else if (dynamic_cast<BooleanConstNode *>(node.get()))
    {
        auto c = dynamic_cast<BooleanConstNode *>(node.get());
        auto bool_type = compiler_context.get_boolean_type();
        auto value = llvm::ConstantInt::get(llvm::Type::getInt1Ty(context), c->value.type == TokenType::True ? 1 : 0);
        return {this->block, bool_type.set_loaded_value(value, builder)};
    }
    else if (dynamic_cast<StringNode *>(node.get()))
    {
        auto c = dynamic_cast<StringNode *>(node.get());

        auto a = compiler_context.get_string_type(context, builder);
        llvm::Value *structPtr = builder.CreateAlloca(a);
        auto str = builder.CreateGlobalString(c->value);
        auto length = llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), c->value.size() + 1);
        auto value = this->CompileStr(str, length, structPtr);
        auto str_optional_type = compiler_context.get_string_type();

        return {this->block, str_optional_type.set_loaded_value(value, builder)};
    }
    else if (dynamic_cast<FunctionCallNode *>(node.get()))
    {
        auto c = dynamic_cast<FunctionCallNode *>(node.get());
        auto fu = program.get_function(c->name);
        auto function_call = builder.CreateCall(fu.function, {});
        OptionalType type_of_func = compiler_context.get_type(fu.ret_type);
        return {this->block, type_of_func.set_loaded_value(function_call, builder)};
    }
    else if (dynamic_cast<ExprNode *>(node.get()))
    {
        auto c = dynamic_cast<ExprNode *>(node.get());

        auto lhs = Expression(c->lhs);

        auto rhs = Expression(c->rhs);
        auto get_type = get_expr_type(node, this->program);
        switch (get_type)
        {
        case Integer_Type:
            return {this->block, IntegerMath(lhs.value, c->operation, rhs.value)};
        case Float_Type:
            return {this->block, FloatMath(lhs.value, c->operation, rhs.value)};
        case String_Type:
            return {this->block, StringMath(lhs.value, c->operation, rhs.value)};
        default:
            break;
        }
    }
    else if (dynamic_cast<BooleanExprNode *>(node.get()))
    {
        auto c = dynamic_cast<BooleanExprNode *>(node.get());

        auto lhs = Expression(c->lhs);
        auto rhs = Expression(c->rhs);
        auto get_type = get_bool_expr_type(node, this->program);
        switch (get_type)
        {
        case Integer_Type:
            return {this->block, IntegerBool(lhs.value, c->op, rhs.value)};
        case Float_Type:
            return {this->block, FloatBool(lhs.value, c->op, rhs.value)};
        default:
            break;
        }
    }
    return {this->block, nullptr};
}
