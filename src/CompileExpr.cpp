#include <backend/compiler_visitors.h>
#include <backend/CompilerUtil.h>
#include <backend/CompilerContext.h>

// This compiles expresssions
// it doesnt use the visitor pattern as thats cursed so
// IE using a stack so what I did was basic.
// TODO: refactor
CompileExpr::CompileExpr(llvm::Module &module,
                         llvm::IRBuilder<> &builder,
                         llvm::LLVMContext &context,
                         CompilerContext compiler_context,
                         ProgramScope program,
                         llvm::BasicBlock *block, llvm::StructType *params) : module(module),
                                                                              builder(builder),
                                                                              context(context)
{
    this->compiler_context = compiler_context;
    this->program = program;
    this->block = block;
    this->params = params;

    // this->func_map = func_map;
}
llvm::Value *CompileExpr::CompileStr(llvm::Value *str, llvm::Value *length, llvm::Value *structure)
{

    // strings are strcutures
    // that conatin  the string and length

    auto c = compiler_context.get_string_inner_type();
    auto destField0ptr = builder.CreateStructGEP(c, structure, 0, "destStructPtrF0");
    builder.CreateStore(str, destField0ptr);
    // structure->getType()->dump();
    // str->getType()->dump();
    // length->getType()->dump();
    auto destField1ptr = builder.CreateStructGEP(c, structure, 1, "destStructPtrF1");
    // length = builder.CreateLoad(builder.getInt64Ty(), length);
    builder.CreateStore(length, destField1ptr);
    // return ValueOrLoad(builder, structure, c);
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
    // auto lhs_val = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(integer_type.type, lhs, 0, "int_lhs"));
    // auto rhs_val = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(integer_type.type, rhs, 0, "int_rhs"));
    auto lhs_val = integer_type.get_inner_value(builder, lhs, true);
    auto rhs_val = integer_type.get_inner_value(builder, rhs, true);

    // builder.CreateCall(compiler_context.CFunctions["printf"], {builder.CreateGlobalString("[HAZELC DEBUG]: %d \n"),
    //    rhs_val});

    // builder.CreateCall(compiler_context.CFunctions["printf"], {builder.CreateGlobalString("[HAZELC DEBUG]: %d \n"),
    //    lhs_val});
    auto math = IntMathExpression(lhs_val, op, rhs_val);
    // builder.CreateCall(compiler_context.CFunctions["printf"], {builder.CreateGlobalString("[HAZELC DEBUG]: %d \n"),
    //    math});
    return integer_type.set_loaded_value(math, builder);
}
llvm::Value *CompileExpr::FloatMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{

    auto float_type = compiler_context.get_float_type();

    auto lhs_val = float_type.get_inner_value(builder, lhs, true);
    auto rhs_val = float_type.get_inner_value(builder, rhs, true);
    // auto lhs_val = builder.CreateLoad(builder.getDoubleTy(), builder.CreateStructGEP(float_type.type, lhs, 0, "float_lhs"));
    // auto rhs_val = builder.CreateLoad(builder.getDoubleTy(), builder.CreateStructGEP(float_type.type, rhs, 0, "float_rhs"));
    auto math = FloatMathExpression(lhs_val, op, rhs_val);
    return float_type.set_loaded_value(math, builder);
}
llvm::Value *CompileExpr::StringMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{

    auto string_type = compiler_context.get_string_type();
    auto string_inner_type = compiler_context.get_string_inner_type();

    auto lhs_val = string_type.get_inner_value(builder, lhs, false);
    auto rhs_val = string_type.get_inner_value(builder, rhs, false);
    // auto lhs_val = builder.CreateStructGEP(string_type.type, lhs, 0, "str_lhs");
    // auto rhs_val = builder.CreateStructGEP(string_type.type, rhs, 0, "str_rhs");
    auto math = StringMathExpr(lhs_val, op, rhs_val);
    auto ret = string_type.set_loaded_struct_value(compiler_context.CProcedures.memcpy, math, builder, string_type.get_inner_size(module));
    return ret;

    // debug print
    //  auto str_lhs_test = builder.CreateLoad(builder.getInt8PtrTy(), builder.CreateStructGEP(string_inner_type, math, 0));
    //  return string_type.set_loaded_struct_value(math, builder);
    //  return string_type.set_loaded_value(math, builder);
    //  auto string

    // lhs_val = builder.CreateStructGEP(string_type.type, ret, 0, "str_res");
    // str_lhs_test = builder.CreateLoad(builder.getInt8PtrTy(), builder.CreateStructGEP(string_inner_type, lhs_val, 0));
    // builder.CreateCall(compiler_context.CFunctions["printf"], {builder.CreateGlobalString("[HAZELC DEBUG]: concat result after we put it in a different struct, you are in the g function: %s \n"),
    //    str_lhs_test});
    // builder.CreateCall(compiler_context.CFunctions["printf"], {builder.CreateGlobalString("[HAZELC DEBUG]: concat resultant len: %d \n"),
    //    builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(string_inner_type, lhs_val, 1))});
}

llvm::Value *CompileExpr::StringBoolMath(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    auto c = compiler_context.get_string_inner_type();

    auto strRhsPtr = builder.CreateLoad(builder.getInt8PtrTy(), builder.CreateStructGEP(c, rhs, 0, "strrhsval"));
    auto strLhsPtr = builder.CreateLoad(builder.getInt8PtrTy(), builder.CreateStructGEP(c, lhs, 0, "strlhsval"));
    auto streq = compiler_context.CProcedures.strcmp;

    auto expr = builder.CreateCall(streq, {strLhsPtr, strRhsPtr});
    // builder.CreateCall(compiler_context.CProcedures.printf, {builder.CreateGlobalString("[HAZELC DEBUG]: foos str %s\n"),
    //                                                          strLhsPtr});
    // auto strLhsLen = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(c, lhs, 1, "strrhsval"));

    // builder.CreateCall(compiler_context.CFunctions["printf"], {builder.CreateGlobalString("[HAZELC DEBUG]: foo length %d \n"),
    //                                                            strLhsLen});

    // builder.CreateCall(compiler_context.CFunctions["printf"], {builder.CreateGlobalString("[HAZELC DEBUG]: %d \n"),
    //                                                            expr});
    switch (op.type)
    {
    case EQ:
    {
        return builder.CreateICmp(llvm::ICmpInst::ICMP_EQ, expr, llvm::ConstantInt::get(builder.getInt32Ty(), 0));
    }
    case NE:
    {
        return builder.CreateICmp(llvm::ICmpInst::ICMP_NE, expr, llvm::ConstantInt::get(builder.getInt32Ty(), 0));
    }
    default:
        break;
    }
    return nullptr;
}

llvm::Value *CompileExpr::IntegerBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    auto integer_type = compiler_context.get_integer_type();
    auto BoolType = compiler_context.get_boolean_type();
    auto lhs_val = integer_type.get_inner_value(builder, lhs, true);
    auto rhs_val = integer_type.get_inner_value(builder, rhs, true);
    // auto lhs_val = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(integer_type.type, lhs, 0, "int_lhs"));
    // auto rhs_val = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(integer_type.type, rhs, 0, "int_rhs"));
    auto math = BoolIntMathExpr(lhs_val, op, rhs_val);
    return BoolType.set_loaded_value(math, builder);
}

llvm::Value *CompileExpr::FloatBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    auto float_type = compiler_context.get_float_type();
    auto BoolType = compiler_context.get_boolean_type();

    auto lhs_val = float_type.get_inner_value(builder, lhs, true);
    auto rhs_val = float_type.get_inner_value(builder, rhs, true);

    // auto lhs_val = builder.CreateLoad(builder.getDoubleTy(), builder.CreateStructGEP(float_type.type, lhs, 0, "float_lhs"));
    // auto rhs_val = builder.CreateLoad(builder.getDoubleTy(), builder.CreateStructGEP(float_type.type, rhs, 0, "float_rhs"));
    auto math = BoolFloatMathExpr(lhs_val, op, rhs_val);

    return BoolType.set_loaded_value(math, builder);
}

llvm::Value *CompileExpr::BoolBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{

    auto BoolType = compiler_context.get_boolean_type();
    // auto lhs_val = builder.CreateLoad(builder.getInt1Ty(), builder.CreateStructGEP(BoolType.type, lhs, 0, "bool_lhs"));
    // auto rhs_val = builder.CreateLoad(builder.getInt1Ty(), builder.CreateStructGEP(BoolType.type, rhs, 0, "bool_rhs"));
    auto lhs_val = BoolType.get_inner_value(builder, lhs, true);
    auto rhs_val = BoolType.get_inner_value(builder, rhs, true);

    auto math = BoolIntMathExpr(lhs_val, op, rhs_val);
    return BoolType.set_loaded_value(math, builder);
}
llvm::Value *CompileExpr::NoneBool(llvm::Value *lhs, Tokens op, llvm::Value *rhs, BooleanExprNode *nodE)
{
    auto BoolType = compiler_context.get_boolean_type();
    if (getTypeOfOnSide(nodE->lhs, compiler_context).has_value())
    {
        auto lhs_val = builder.CreateLoad(builder.getInt1Ty(), builder.CreateStructGEP(getTypeOfOnSide(nodE->lhs, compiler_context).value().type, lhs, 1, "lhs"));
        auto rhs_val = builder.getInt1(true);

        auto math = BoolIntMathExpr(lhs_val, op, rhs_val);
        return BoolType.set_loaded_value(math, builder);
    }
    else if (getTypeOfOnSide(nodE->rhs, compiler_context).has_value())
    {
        auto rhs_val = builder.CreateLoad(builder.getInt1Ty(),
                                          builder.CreateStructGEP(getTypeOfOnSide(nodE->rhs, compiler_context).value().type, rhs, 1, "rhs"));
        auto lhs_val = builder.getInt1(true);
        auto math = BoolIntMathExpr(lhs_val, op, rhs_val);
        return BoolType.set_loaded_value(math, builder);
    }
    else
    {
        auto lhs_val = builder.CreateLoad(builder.getInt1Ty(), builder.CreateStructGEP(getTypeOfOnSide(nodE->lhs, compiler_context).value().type, lhs, 1, "lhs"));
        auto rhs_val = builder.CreateLoad(builder.getInt1Ty(), builder.CreateStructGEP(getTypeOfOnSide(nodE->rhs, compiler_context).value().type, rhs, 1, "rhs"));

        auto math = BoolIntMathExpr(lhs_val, op, rhs_val);
        return BoolType.set_loaded_value(math, builder);
    }
}
ValueStruct CompileExpr::CompileBranch(std::shared_ptr<BlockNode> b)
{
    return Expression(b->exit->Expr);
}

ValueStruct CompileExpr::CompileConditional(ConditionalNode *condition_stmnt)
{

    auto type = compiler_context.get_type(condition_stmnt->type);
    std::vector<std::tuple<llvm::BasicBlock *, llvm::Value *>> phi_nodes;
    llvm::BasicBlock *endTrue = llvm::BasicBlock::Create(context, "end.true", compiler_context.get_current_function().function);
    for (int i = 0; i < condition_stmnt->branches.size(); i++)
    {
        auto condition = Expression(condition_stmnt->branches[i]->condition).value;
        llvm::BasicBlock *ifTrue = llvm::BasicBlock::Create(context, "if.true", compiler_context.get_current_function().function);
        condition = builder.CreateLoad(builder.getInt1Ty(),
                                       builder.CreateStructGEP(compiler_context.get_boolean_type().type, condition, 0, "str2"));
        if (i < condition_stmnt->branches.size() - 1)
        {
            llvm::BasicBlock *ElsTrue = llvm::BasicBlock::Create(context, "else.true", compiler_context.get_current_function().function);
            builder.CreateCondBr(condition, ifTrue, ElsTrue);
            builder.SetInsertPoint(ifTrue);
            this->block = ifTrue;
            auto value = CompileBranch(condition_stmnt->branches[i]->stmnts);
            // auto value =
            // auto loaded_val = ValueOrLoad(builder, value.value, type.get_type());

            phi_nodes.push_back({value.block, value.value});
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
            // auto loaded_val = ValueOrLoad(builder, value.value, type.get_type());
            phi_nodes.push_back({value.block, value.value});
            builder.CreateBr(endTrue);
        }
    }
    builder.SetInsertPoint(endTrue);
    this->block = endTrue;
    llvm::PHINode *phi = builder.CreatePHI(llvm::PointerType::get(type.type, 0), phi_nodes.size(), "iftmp");
    for (int i = 0; i < phi_nodes.size(); i++)
    {
        phi->addIncoming(std::get<1>(phi_nodes[i]), std::get<0>(phi_nodes[i]));
    }
    return {endTrue, phi};
    // return ;
}

llvm::Value *CompileExpr::FloatMathExpression(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    switch (op.type)
    {
    case Addition:
        return builder.CreateFAdd(lhs, rhs, "addition");
    case Multiplication:
        return builder.CreateFMul(lhs, rhs, "multiply");
    case Division:
        return builder.CreateFDiv(lhs, rhs, "division");
    case Subtraction:
        return builder.CreateFSub(lhs, rhs, "subtract");
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
    case And:
        return builder.CreateAnd(lhs, rhs);
    case Or:
        return builder.CreateOr(lhs, rhs);
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

llvm::Value *CompileExpr::BoolBoolExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    return nullptr;
}

llvm::Value *CompileExpr::StringMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    switch (op.type)
    {
    case Concation:
    {
        // DEBUG PRINTS
        //  builder.CreateCall(compiler_context.CFunctions["printf"], {builder.CreateGlobalString("[HAZELC DEBUG]: str-left-hand: %s \n"),
        //     strLhsPtr});

        // builder.CreateCall(compiler_context.CFunctions["printf"], {builder.CreateGlobalString("[HAZELC DEBUG]: str-right-hand: %s \n"),
        //    strRhsPtr});

        // rn to do concat we use llvm.memcpy
        // defined as
        // void* memcpy(void* dest, void* src, size_t size);
        // malloc defined as
        // void *malloc(size_t bytes)
        // we use malloc to allocate a buffer
        // we then use the llvm.memcpy the first half to the buffer
        // increment buffer
        // and llvm.memcpy the 2nd half to the buffer
        // auto memcpy = compiler_context.CFunctions["memcpy"];

        auto c = compiler_context.get_string_inner_type();

        auto strRhsPtr = builder.CreateLoad(builder.getInt8PtrTy(), builder.CreateStructGEP(c, rhs, 0, "strlhsval"));
        auto strLhsPtr = builder.CreateLoad(builder.getInt8PtrTy(), builder.CreateStructGEP(c, lhs, 0, "strrhsval"));
        auto lenthlhs = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(c, lhs, 1, "strlenlhs"));
        auto lenthrhs = builder.CreateLoad(builder.getInt64Ty(), builder.CreateStructGEP(c, rhs, 1, "strlnrhs"));
        auto added_lengths = builder.CreateAdd(lenthlhs, lenthrhs);
        // auto dest = builder.BuildArrayMalloc()
        auto dest = builder
                        .CreateCall(compiler_context.CProcedures.malloc, {added_lengths});

        // builder.CreateMemCpy(dest,
        //                      strLhsPtr->getAlign(),
        //                      strLhsPtr,
        //                      strLhsPtr->getAlign(),
        //                      lenthlhs);
        builder.CreateCall(compiler_context.CProcedures.memcpy, {dest, strLhsPtr, lenthlhs, builder.getInt1(0)});

        auto second_dest = builder.CreateInBoundsGEP(builder.getInt8Ty(), dest, {lenthlhs});

        builder.CreateCall(compiler_context.CProcedures.memcpy, {second_dest, strRhsPtr, lenthrhs, builder.getInt1(0)});
        // builder.CreateMemCpy(second_dest,
        //                      strRhsPtr->getAlign(),
        //                      strRhsPtr,
        //                      strRhsPtr->getAlign(),
        //                      lenthrhs);

        // DEBUG PRINTS
        // builder.CreateCall(compiler_context.CProcedures.printf, {builder.CreateGlobalString("[HAZELC DEBUG]: concat result: %s \n"),
        //                                                          dest});

        return this->CompileStr(dest, added_lengths, builder.CreateAlloca(c));
    }
    default:
    {
        std::cout << "hazelc: semantic analysis bug perhaps in string operators \"" << op.value << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
    }
    // DEBUG STRCAT:
}

llvm::Value *CompileExpr::StringBoolMathExpr(llvm::Value *lhs, Tokens op, llvm::Value *rhs)
{
    auto string_type = compiler_context.get_string_type();
    auto inner = compiler_context.get_string_inner_type();
    auto BoolType = compiler_context.get_boolean_type();

    auto lhs_val = builder.CreateStructGEP(string_type.type, lhs, 0, "str_lhs");
    auto rhs_val = builder.CreateStructGEP(string_type.type, rhs, 0, "str_rhs");
    auto math = StringBoolMath(lhs_val, op, rhs_val);
    return BoolType.set_loaded_value(math, builder);
}

ValueStruct CompileExpr::Expression(std::shared_ptr<ASTNode> node)
{
    if (dynamic_cast<IntegerNode *>(node.get()))
    {

        auto c = dynamic_cast<IntegerNode *>(node.get());
        auto get_int_type = compiler_context.get_integer_type();
        auto number = builder.getInt64(c->number);
        return {this->block, get_int_type.set_loaded_value(number, builder)};
    }
    else if (dynamic_cast<NoneNode *>(node.get()))
    {
        return {
            this->block, compiler_context.get_type(compiler_context.get_current_function().ret_type).get_none(builder)};
    }
    else if (dynamic_cast<ConditionalNode *>(node.get()))
    {
        auto condition_stmnt = dynamic_cast<ConditionalNode *>(node.get());
        return CompileConditional(condition_stmnt);
    }
    else if (dynamic_cast<CharNode *>(node.get()))
    {

        auto c = dynamic_cast<CharNode *>(node.get());
        auto char_type = compiler_context.get_byte_type();
        auto value = builder.getInt8(c->value.value[0]);

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
        auto value = llvm::ConstantInt::get(builder.getInt1Ty(), c->val);
        return {this->block, bool_type.set_loaded_value(value, builder)};
    }
    else if (dynamic_cast<StringNode *>(node.get()))
    {
        auto c = dynamic_cast<StringNode *>(node.get());

        auto a = compiler_context.get_string_inner_type();
        auto str = builder.CreateGlobalString(c->value, c->value + " contents");
        // auto length = llvm::ConstantInt::get(builder.getInt64Ty(), c->value.size());
        auto length = builder.getInt64(c->value.size());
        llvm::Value *structPtr = builder.CreateAlloca(a);
        auto value = this->CompileStr(str, length, structPtr);
        auto str_optional_type = compiler_context.get_string_type();
        return {this->block,
                str_optional_type
                    .set_loaded_struct_value(
                        compiler_context.CProcedures.memcpy,
                        value, builder, str_optional_type.get_inner_size(module))};
    }
    else if (dynamic_cast<FunctionCallNode *>(node.get()))
    {
        auto c = dynamic_cast<FunctionCallNode *>(node.get());

        auto fu = compiler_context.get_function(c->hash_name.has_value() ? c->hash_name.value() : c->name.value).value();
        // std::cout << fu.name.value << std::endl;

        // auto v =
        llvm::Value *param_ptr = builder.CreateAlloca(this->params);
        OptionalType type_of_func = compiler_context.get_type(fu.ret_type);
        auto retTy = builder.CreateAlloca(type_of_func.get_type());
        auto function_call = builder.CreateCall(fu.function, {param_ptr, retTy});
        function_call->addParamAttr(1, llvm::Attribute::getWithStructRetType(context, type_of_func.get_type()));
        // auto val = builder.CreateStructGEP(type_of_func.type, function_call, 0);
        // val = ValueOrLoad(builder, val, type_of_func.inner);
        // auto rhs_struct_ptr = builder.Crea(function_call, ptrType);
        // function_call->dump();
        // auto s = ValueOrLoad(builder, function_call, type_of_func.type);
        // auto func_call = builder.CreateLoad(type_of_func.inner,
        // builder.CreateStructGEP(ptrType, function_call, 0, "func_call");
        // func_call->getType()->dump();
        // type_of_func.type->dump();
        // auto val = builder.CreateStructGEP(type_of_func.type, s, 0);
        // val->getType()->dump();
        return {this->block, retTy};
    }
    else if (dynamic_cast<ExprNode *>(node.get()))
    {

        auto c = dynamic_cast<ExprNode *>(node.get());

        auto lhs = Expression(c->lhs);

        auto rhs = Expression(c->rhs);
        auto get_type = get_expr_type(node, this->compiler_context);

        switch (get_type)
        {
        case Integer_Type:
            return {this->block, IntegerMath(lhs.value, c->operation, rhs.value)};
        case Float_Type:
            return {this->block, FloatMath(lhs.value, c->operation, rhs.value)};
        case String_Type:
            return {this->block, StringMath(lhs.value, c->operation, rhs.value)};
        case Boolean_Type:
            return {this->block, BoolBool(lhs.value, c->operation, rhs.value)};
        case None_Type:
            std::cout << "none" << std::endl;
            break;
        default:
            break;
        }
    }
    else if (dynamic_cast<BooleanExprNode *>(node.get()))
    {
        auto c = dynamic_cast<BooleanExprNode *>(node.get());

        auto lhs = Expression(c->lhs);
        auto rhs = Expression(c->rhs);

        auto get_type = get_bool_expr_type(node, this->compiler_context);
        std::cout << get_type << std::endl;
        switch (get_type)
        {
        case Integer_Type:
            return {this->block, IntegerBool(lhs.value, c->op, rhs.value)};
        case Float_Type:
            return {this->block, FloatBool(lhs.value, c->op, rhs.value)};
        case String_Type:
            return {this->block, StringBoolMathExpr(lhs.value, c->op, rhs.value)};
        case None_Type:
            return {this->block, NoneBool(lhs.value, c->op, rhs.value, c)};
            break;
        case Boolean_Type:
            return {this->block, BoolBool(lhs.value, c->op, rhs.value)};
        default:
            break;
        }
    }
    return {this->block, nullptr};
}
