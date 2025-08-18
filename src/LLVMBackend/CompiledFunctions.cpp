#include <backend/Compiled_Functions.h>
#include <backend/CompilerContext.h>

// They are 3 types of functions
// - parameters which are Memoized and are lazy eval. we must handle them differenltly
// - normal function. we cann call how we call a function typically
Compiled_Function::Compiled_Function()
{
}
DefinedFunction::DefinedFunction()
{
}
DefinedFunction::DefinedFunction(Function function)
{
    this->function = function;
}
std::shared_ptr<Type> DefinedFunction::get_ret_type()
{
    return function.ret_type;
}
ValueStruct DefinedFunction::compile(CompilerContext &ctx, llvm::BasicBlock *block, llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context)

{
    llvm::Value *param_ptr = builder.CreateAlloca(ctx.params);

    OptionalType type_of_func = ctx.get_type(this->function.ret_type);
    auto retTy = builder.CreateAlloca(type_of_func.get_type());
    // fu.

    llvm::DataLayout datalayout(&module);
    auto p_size = datalayout.getTypeAllocSize(ctx.params);
    // std::cout << ctx. << std::endl;

    for (int i = 0; i < this->function.thunks.size(); i++)
    {
        auto functionParam = builder.CreateStructGEP(ctx.params, param_ptr, this->function.thunks[i].gep_loc, "destStructPtrF0");

        auto isComputed = builder.CreateStructGEP(this->function.thunks[i].thunk_type, functionParam, 2, "isComputed");
        auto functionPtrField = builder.CreateStructGEP(this->function.thunks[i].thunk_type, functionParam, 1, "functionPtr");
        builder.CreateStore(builder.getInt1(0), isComputed);
        builder.CreateStore(ctx.function_params.front().function, functionPtrField);
        // std::cout << "PARAM: " << ctx.function_params.front().name.value << std::endl;
        // std::cout << ""
        // auto dest = builder.CreateCall(ctx.CProcedures.malloc, {builder.getInt64(p_size)});

        // builder.CreateCall(ctx.CProcedures.memcpy, {functionParamInStruct, functionParam, builder.getInt64(p_size), builder.getInt1(0)});
        ctx.function_params.pop();
    }

    auto function_call = builder.CreateCall(this->function.function, {param_ptr, retTy});
    function_call->addParamAttr(1, llvm::Attribute::getWithStructRetType(context, type_of_func.get_type()));
    return {block, retTy};
}

ParamFunction::ParamFunction()
{
}
ParamFunction::ParamFunction(FunctionParam thunk)
{
    this->thunk = thunk;
}

std::shared_ptr<Type> ParamFunction::get_ret_type()
{
    return this->thunk.ret_type;
}
ValueStruct ParamFunction::compile(CompilerContext &ctx, llvm::BasicBlock *block, llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context)
{

    auto f = ctx.get_current_function().function;
    llvm::Argument *param_ptr = f->getArg(0);

    auto destField0ptr = builder.CreateStructGEP(ctx.params, param_ptr, thunk.gep_loc, "destStructPtrF0");
    auto actualVal = builder.CreateStructGEP(thunk.thunk_type, destField0ptr, 0, "actualVal");
    auto functionPtrField = builder.CreateStructGEP(thunk.thunk_type, destField0ptr, 1, "functionPtr");
    // auto isComputed = builder.CreateStructGEP(thunk.thunk_type, destField0ptr, 2, "isComputed");
    // auto param_in_thunk = builder.CreateStructGEP(thunk.thunk_type, destField0ptr, 3, "param_ptr");

    auto functionPtr = builder.CreateLoad(thunk.type->getPointerTo(), functionPtrField, "loadedFuncPtr");
    // // llvm::BasicBlock *ifTrue = llvm::BasicBlock::Create(context, "if.value.exists", ctx.get_current_function().function);
    // // llvm::BasicBlock *endTrue = llvm::BasicBlock::Create(context, "end.value.exists", ctx.get_current_function().function);
    // // auto cond = builder.CreateICmp(llvm::ICmpInst::ICMP_EQ, builder.CreateLoad(builder.getInt1Ty(), isComputed), builder.getInt1(0));
    // // builder.CreateCondBr(cond, ifTrue, endTrue);
    // // builder.SetInsertPoint(ifTrue);
    OptionalType retType = ctx.get_type(thunk.ret_type);
    auto retTy = builder.CreateAlloca(retType.type);
    // functionPtr->dump();
    auto v = builder.CreateCall(thunk.type, functionPtr, {param_ptr, retTy});
    OptionalType type_of_func = ctx.get_type(this->thunk.ret_type);

    v->addParamAttr(1, llvm::Attribute::getWithStructRetType(context, type_of_func.get_type()));

    // builder.CreateStore(builder.getInt1(1), isComputed);
    // // builder.CreateStore(retTy, actualVal);

    builder.CreateCall(ctx.CProcedures.memcpy, {
                                                   actualVal,
                                                   retTy,
                                                   llvm::ConstantInt::get(builder.getInt64Ty(), retType.get_type_size(module)),
                                                   llvm::ConstantInt::get(builder.getInt1Ty(), 0),

                                               });

    // builder.CreateBr(endTrue);
    // builder.SetInsertPoint(endTrue);
    return {block, actualVal};
}
