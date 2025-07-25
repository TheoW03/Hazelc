#include <backend/compiler_visitors.h>
#include <backend/CompilerUtil.h>
#include <backend/CompilerContext.h>
#include <llvm/IR/Verifier.h>
#include <backend/Compiled_Functions.h>
// now it goes through the data structure generated by CompielHighLevel and
// compiles the lower level (so return statements and expressions)
// this is the final stage
CompileStatement::CompileStatement(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context,
                                   CompilerContext compiler_context, llvm::StructType *params) : module(module), builder(builder), context(context)
{
    this->compiler_context = compiler_context;
    this->params = params;
}

void CompileStatement::Visit(ASTNode *node)
{
}

void CompileStatement::Visit(FunctionNode *node)
{

    // all functions are stored in a stack. the compiler pulls a function from the stack
    // sees if its a global or local function
    // if local we put it in the local scope. if global we generate it
    auto current_function = this->compiler_context.set_current_function();
    if (!current_function.isAnonymous && !node->hash_name.has_value())
    {
        // compiler_context.addLocal(current_function.name, current_function);
        compiler_context.addLocal(node->f->FunctionName, std::make_shared<DefinedFunction>(current_function));
    }
    llvm::BasicBlock *EntryBlock = llvm::BasicBlock::Create(context, node->f->FunctionName.value + " entry", current_function.function);
    this->block = EntryBlock;
    builder.SetInsertPoint(EntryBlock);
    // add the parameters

    for (int i = 0; i < current_function.thunks.size(); i++)
    {
        compiler_context.addLocal(current_function.thunks[i].name, std::make_shared<ParamFunction>(current_function.thunks[i]));
    }

    node->stmnts->exit->Accept(this);
}

void CompileStatement::Visit(DemoduarlizedProgramNode *node)
{

    auto functions = node->functions;
    std::reverse(functions.begin(), functions.end());
    for (int i = 0; i < node->functions.size(); i++)
    {
        functions[i]->Accept(this);
    }
    std::cout << "hazelc: Compiled Expressions" << std::endl;
}

void CompileStatement::Visit(ReturnNode *node)
{

    CompileExpr c(module, builder, context, compiler_context, this->block);

    auto ty = compiler_context.get_type(compiler_context.get_current_function().ret_type);
    // auto value = builder.CreateLoad(ty.get_type(), c.Expression(node->Expr));
    // program_scope.get_current_function().function->viewCFGOnly();
    llvm::raw_ostream *output = &llvm::outs();

    auto value = c.Expression(node->Expr);
    this->block = value.block;
    auto f = compiler_context.get_current_function().function;
    llvm::Argument *ret_ptr = f->getArg(1);
    if (compiler_context.get_current_function().name.value != "main")
    {

        // llvm::Function *MemcpyFunc = llvm::Intrinsic::getDeclaration(
        //     &module,
        //     llvm::Intrinsic::memcpy,
        //     {
        //         builder.getPtrTy(),
        //         builder.getPtrTy(),
        //         builder.getInt64Ty(),
        //     });

        builder.CreateCall(compiler_context.CProcedures.memcpy, {
                                                                    ret_ptr,
                                                                    value.value,
                                                                    llvm::ConstantInt::get(builder.getInt64Ty(), ty.get_type_size(module)),
                                                                    llvm::ConstantInt::get(builder.getInt1Ty(), 0),

                                                                });

        // llvm::Value *optionalStructField0 = builder.CreateStructGEP(ty.get_type(), ret_ptr, 0, "OptionalStructPtr0");
        // llvm::Value *optionalStructField1 = builder.CreateStructGEP(ty.get_type(), ret_ptr, 1, "OptionalStructPtr1");

        // Copy the value (i.e., the %string) into the OptionalType.6's first field
        // builder.CreateStore(value.value, optionalStructField0);

        // Set the "isNone" boolean to false
        // auto isNone = llvm::ConstantInt::get(builder.getInt1Ty(), 0);
        // builder.CreateStore(isNone, optionalStructField1);
        // llvm::Value *structPtr = builder.CreateAlloca(ty.get_type());
        // auto destField0ptr = builder.CreateStructGEP(ty.get_type(), structPtr, 0, "OptionalStructPtr0");

        // builder.CreateStore(value.value, destField0ptr);
        // auto destField1ptr = builder.CreateStructGEP(ty.get_type(), structPtr, 1, "OptionalStructPtr1");
        // auto isNone = llvm::ConstantInt::get(builder.getInt1Ty(), 0);
        // builder.CreateStore(isNone, destField1ptr);

        // builder.CreateStore(value.value, ret_ptr);
    }

    builder.CreateRetVoid();
    auto error = llvm::verifyFunction(*(f), output);
}
