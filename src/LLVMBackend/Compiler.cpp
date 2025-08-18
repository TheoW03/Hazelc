
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
#include <backend/compiler_visitors.h>
// #include "llvm/Support/Host.h"
#include "llvm/TargetParser/Host.h"
#include <fstream>
#include "llvm/IR/LLVMContext.h"
#include <cli.h>

void InitCompiler(Output output, std::shared_ptr<DemoduarlizedProgramNode> node)
{

    llvm::LLVMContext context;
    llvm::Module module("MyModule", context);

    llvm::IRBuilder<> builder(context);

    std::map<std::string, llvm::Function *> func_map;
    llvm::DataLayout datalayout(&module);

    size_t size_test = datalayout.getTypeAllocSize(builder.getInt32Ty());
    // CompileHighLevel c(module, builder, context);
    // compiles function body
    CompileHighLevel *compile_top = new CompileHighLevel(module, builder, context);
    node->Accept(compile_top);
    compile_top->params->setBody(compile_top->params_struct);
    compile_top->compiler_context.set_params(compile_top->params);
    CompileStatement *compile_statement = new CompileStatement(module, builder, context, compile_top->compiler_context, compile_top->params);
    node->Accept(compile_statement);
    std::cout << "" << std::endl;
    delete compile_top;

    /*
        llvm::LLVMContext context;
        llvm::Module module("func_ptr_demo", context);
        llvm::IRBuilder<> builder(context);

        // --- Step 1: Create a function: int foo()
        llvm::FunctionType *fooTy =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
        llvm::Function *fooFn = llvm::Function::Create(
            fooTy, llvm::Function::ExternalLinkage, "foo", module);

        llvm::BasicBlock *fooEntry =
            llvm::BasicBlock::Create(context, "entry", fooFn);
        builder.SetInsertPoint(fooEntry);
        // builder.CreateRet(llvm::CreateCall::get(llvm::Type::getInt32Ty(context), 42));
        builder.CreateRet(builder.CreateCall(fooFn, {}));
        // --- Step 2: Struct with a function pointer field
        llvm::PointerType *fooPtrTy = fooTy->getPointerTo();
        llvm::StructType *structTy =
            llvm::StructType::create(context, {fooPtrTy}, "MyStruct");

        // --- Step 3: Define main() that stores and calls function pointer
        llvm::FunctionType *mainTy =
            llvm::FunctionType::get(llvm::Type::getInt32Ty(context), false);
        llvm::Function *mainFn = llvm::Function::Create(
            mainTy, llvm::Function::ExternalLinkage, "main", module);

        llvm::BasicBlock *mainEntry =
            llvm::BasicBlock::Create(context, "entry", mainFn);
        builder.SetInsertPoint(mainEntry);

        // Allocate the struct
        llvm::Value *alloc = builder.CreateAlloca(structTy, nullptr, "s");

        // GEP to the function pointer field (index 0)
        llvm::Value *fieldPtr =
            builder.CreateStructGEP(structTy, alloc, 0, "fp_slot");

        // Store function pointer into struct
        builder.CreateStore(fooFn, fieldPtr);

        // Load function pointer
        llvm::Value *loadedFn =
            builder.CreateLoad(fooPtrTy, fieldPtr, "fp_loaded");

        // Call through the pointer
        loadedFn->dump();
        llvm::CallInst *callRes =
            builder.CreateCall(fooTy, loadedFn, {});
        builder.CreateRet(callRes);

        // --- Verify & print IR
        llvm::verifyModule(module, &llvm::errs());
        module.print(llvm::outs(), nullptr); // Initialize the target registry etc.llvm::InitializeAllTargets();
    */
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
            std::system("rm a.o");

            dest.flush();

            break;
        }
        }
    }
}
