
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include <iostream>
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"

#include "llvm/Support/Host.h"

#include "llvm/IR/LLVMContext.h"

void InitCompiler(std::string file_name)
{
    llvm::LLVMContext context;
    llvm::Module module("my_module", context);
    llvm::IRBuilder<> builder(context);

    llvm::ArrayType *arrayType = llvm::ArrayType::get(builder.getInt8PtrTy(), 10000);

    // sets up the run time

    llvm::GlobalVariable *myGlobal = new llvm::GlobalVariable(
        module,    // Module to insert into
        arrayType, // Type of the variable
        false,     // Is constant? (false = mutable)
        llvm::GlobalValue::ExternalLinkage,
        nullptr, // Linkage type
        "stack");

    llvm::FunctionType *push_type = llvm::FunctionType::get(
        builder.getVoidTy(), {builder.getInt8PtrTy()}, false);
    llvm::FunctionType *pop_type = llvm::FunctionType::get(
        builder.getInt8PtrTy(), {}, false);
    llvm::FunctionType *bottom_type = llvm::FunctionType::get(
        builder.getVoidTy(), {}, false);

    llvm::Function *push_cont = llvm::Function::Create(
        push_type, llvm::Function::ExternalLinkage, "pushContinuation", module);
    llvm::Function *pop_cont = llvm::Function::Create(
        pop_type, llvm::Function::ExternalLinkage, "popContinuation", module);
    llvm::Function *bottom = llvm::Function::Create(
        bottom_type, llvm::Function::ExternalLinkage, "bottom", module);

    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    // Initialize the target registry etc.
    llvm::InitializeAllTargets();

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    // auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    std::string Error;

    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);
    module.setDataLayout(TargetMachine->createDataLayout());
    module.setTargetTriple(TargetTriple);

    auto Filename = "output.o";
    std::error_code EC;
    llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

    if (EC)
    {
        llvm::errs() << "Could not open file: " << EC.message();
        return;
    }

    llvm::legacy::PassManager pass;
    auto FileType = llvm::CodeGenFileType::CGFT_ObjectFile;
    if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
    {
        llvm::errs() << "TargetMachine can't emit a file of this type";
        return;
    }

    pass.run(module);
    dest.flush();

    module.print(llvm::outs(), nullptr);
}