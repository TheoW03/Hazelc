
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
#include <fstream>
#include "llvm/IR/LLVMContext.h"

#include <cli.h>
void InitCompiler(Output output, std::vector<std::shared_ptr<ModuleNode>> node)
{
    llvm::LLVMContext context;
    llvm::Module module("MyModule", context); // Module tied to context

    llvm::IRBuilder<> builder(context); // Builder also tied to context

    std::map<std::string, llvm::Function *> func_map;
    // CompileHighLevel c(module, builder, context);
    // compiles function body
    CompileHighLevel *compile_top = new CompileHighLevel(module, builder, context);
    for (int i = 0; i < node.size(); i++)
    {
        node[i]->Accept(compile_top);
    }
    // compiles the returns
    CompileStatement *compile_statement = new CompileStatement(module, builder, context, compile_top->func_map);
    for (int i = 0; i < compile_top->functions.size(); i++)
    {
        compile_top->functions[i]->Accept(compile_statement);
    }
    delete compile_top;
    delete compile_statement;
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
    if (output.print_llvm == 1)
    {
        module.print(llvm::outs(), nullptr);
    }

    if (!Target)
    {
        std::cout << Error << std::endl;
        return;
    }

    // std::cout << Error << std::endl;
    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);
    module.setDataLayout(TargetMachine->createDataLayout());

    // std::cout << "aaa" << std::endl;

    std::error_code EC;
    for (int i = 0; i < output.output_files.size(); i++)
    {

        llvm::legacy::PassManager pass;

        switch (output.gen_file)
        {
        case Object_file:
        {
            llvm::raw_fd_ostream dest(output.output_files[i], EC, llvm::sys::fs::OF_None);
            if (EC)
            {
                llvm::errs() << "Could not open file: " << EC.message();
                return;
            }

            auto FileType = llvm::CodeGenFileType::CGFT_ObjectFile;

            if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
            {
                llvm::errs() << "TargetMachine can't emit a file of this type";
                return;
            }
            pass.run(module);

            dest.flush();
            break;
        }
        case Assembly_File:
        {
            llvm::raw_fd_ostream dest(output.output_files[i], EC, llvm::sys::fs::OF_None);
            if (EC)
            {
                llvm::errs() << "Could not open file: " << EC.message();
                return;
            }

            auto FileType = llvm::CodeGenFileType::CGFT_AssemblyFile;

            if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
            {
                llvm::errs() << "TargetMachine can't emit a file of this type";
                return;
            }
            pass.run(module);

            dest.flush();
            break;
        }
        case Ir_file:
        {
            std::string irString;
            llvm::raw_string_ostream llvmStringStream(irString);

            std::ofstream outfile(output.output_files[i]);
            module.print(llvmStringStream, nullptr);
            outfile << irString << std::endl;
            std::cout << "ir" << std::endl;
            outfile.close();
            break;
        }
        default:
        {
            llvm::raw_fd_ostream dest("a.o", EC, llvm::sys::fs::OF_None);
            if (EC)
            {
                llvm::errs() << "Could not open file: " << EC.message();
                return;
            }
            auto FileType = llvm::CodeGenFileType::CGFT_ObjectFile;

            if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
            {
                llvm::errs() << "TargetMachine can't emit a file of this type";
                return;
            }
            pass.run(module);
            int returnCode = std::system(("clang a.o -o " + output.output_files[i]).c_str());
            dest.flush();
            break;
        }
        }
    }
}