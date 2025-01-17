
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
#include <visitor.h>
#include "llvm/Support/Host.h"

#include "llvm/IR/LLVMContext.h"

void InitCompiler(std::string file_name, std::vector<std::shared_ptr<ModuleNode>> node)
{
    llvm::LLVMContext context;
    llvm::Module module("MyModule", context); // Module tied to context

    llvm::IRBuilder<> builder(context); // Builder also tied to context

    std::map<std::string, llvm::Function *> func_map;
    // CompileHighLevel c(module, builder, context);
    CompileHighLevel *c = new CompileHighLevel(module, builder, context);
    for (int i = 0; i < node.size(); i++)
    {
        for (int j = 0; j < node[i]->functions.size(); j++)
        {
            node[i]->functions[j]->Accept(c);
        }
    }
    std::cout << "compiled functions" << std::endl;
    CompileStatement *c2 = new CompileStatement(module, builder, context, c->func_map);
    for (int i = 0; i < node.size(); i++)
    {
        for (int j = 0; j < node[i]->functions.size(); j++)
        {
            node[i]->functions[j]->Accept(c2);
        }
    }
    delete c;
    delete c2;
    // Initialize the target registry etc.llvm::InitializeAllTargets();

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    // auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    std::string Error;
    auto TargetTriple = llvm::sys::getDefaultTargetTriple();

    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
    module.setTargetTriple(TargetTriple);

    if (!Target)
    {
        std::cout << Error << std::endl;
        return;
    }

    std::cout << Error << std::endl;
    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);
    module.setDataLayout(TargetMachine->createDataLayout());

    std::cout << "aaa" << std::endl;
    module.print(llvm::outs(), nullptr);

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
}