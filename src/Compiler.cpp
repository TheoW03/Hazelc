
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
// #include <visitor.h>
#include <backend/ComplierVisitors.h>
#include "llvm/Support/Host.h"
#include <fstream>
#include "llvm/IR/LLVMContext.h"
#include <cli.h>

void InitCompiler(Output output, std::shared_ptr<ProgramNode> node)
{
    llvm::LLVMContext context;
    llvm::Module module("MyModule", context);

    llvm::IRBuilder<> builder(context);

    std::map<std::string, llvm::Function *> func_map;

    // CompileHighLevel c(module, builder, context);
    // compiles function body
    CompileHighLevel *compile_top = new CompileHighLevel(module, builder, context);
    node->Accept(compile_top);
    std::cout << "hazelc: compiled functions" << std::endl;
    CompileStatement *compile_statement = new CompileStatement(module, builder, context, compile_top->compiler_context);

    node->Accept(compile_statement);
    delete compile_top;

    // Initialize the target registry etc.llvm::InitializeAllTargets();

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
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

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);
    module.setDataLayout(TargetMachine->createDataLayout());

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
