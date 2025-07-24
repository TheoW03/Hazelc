#include <backend/CompilerUtil.h>

CRunTimeFunctions::CRunTimeFunctions() {}
CRunTimeFunctions::CRunTimeFunctions(llvm::IRBuilder<> &builder, llvm::Module &module)
{
    auto printftype = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy()}, true);

    auto printf_func = llvm::Function::Create(
        printftype, llvm::Function::ExternalLinkage, "printf", module);
    this->printf = printf_func;

    auto streq_type = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy(), builder.getInt8PtrTy()}, false);
    auto streq_func = llvm::Function::Create(
        streq_type, llvm::Function::ExternalLinkage, "strcmp", module);
    this->strcmp = streq_func;

    auto maloc_type = llvm::FunctionType::get(builder.getInt8PtrTy(), {builder.getInt64Ty()}, false);
    auto maloc_func = llvm::Function::Create(
        maloc_type, llvm::Function::ExternalLinkage, "malloc", module);
    this->malloc = maloc_func;

    this->memcpy = llvm::Intrinsic::getDeclaration(
        &module,
        llvm::Intrinsic::memcpy,
        {
            builder.getPtrTy(),
            builder.getPtrTy(),
            builder.getInt64Ty(),
        });
}
