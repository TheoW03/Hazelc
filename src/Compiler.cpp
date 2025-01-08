
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
#include <Frontend/parser.h>
#include <map>
#include "llvm/Support/Host.h"

#include "llvm/IR/LLVMContext.h"
llvm::StructType *myStruct;
std::vector<llvm::Type *> elements;
llvm::Type *compileType(llvm::IRBuilder<> &builder, std::shared_ptr<Type> ty)
{
    if (dynamic_cast<NativeType *>(ty.get()))
    {
        auto p = dynamic_cast<NativeType *>(ty.get());
        if (p->type.type == TokenType::Integer)
        {
            return builder.getInt64Ty();
        }
        else if (p->type.type == TokenType::Decimal)
        {
            return builder.getFloatTy();
        }
    }
    return builder.getVoidTy();
}

void CompileStmnt(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::shared_ptr<ASTNode> node, std::map<std::string, llvm::Function *> &func_map)
{
    using std::make_pair; // here bc im lazy you may use using on the stack level. but lets stick to this
    if (dynamic_cast<FunctionNode *>(node.get()))
    {
        auto p = dynamic_cast<FunctionNode *>(node.get());
        auto c = p->f->RetType;
        llvm::FunctionType *functype = llvm::FunctionType::get(
            compileType(builder, c), {}, false);

        llvm::Function *function = llvm::Function::Create(
            functype, llvm::Function::ExternalLinkage, p->f->FunctionName.value, module);
        elements.push_back(functype);
        func_map.insert(make_pair(p->f->FunctionName.value, function));
    }
}
void InitCompiler(std::string file_name, std::vector<std::shared_ptr<ModuleNode>> node)
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
    std::map<std::string, llvm::Function *> func_map;
    myStruct = llvm::StructType::create(context, "lookup_table");
    for (int i = 0; i < node.size(); i++)
    {
        for (int j = 0; j < node[i]->functions.size(); j++)
        {
            CompileStmnt(module, builder, context, node[i]->functions[j], func_map);
            // CompileStmnt(module, builder, context, node[i]->functions[j], func_map);
        }
    }
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