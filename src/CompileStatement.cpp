#include <visitor.h>
#include <backend/CompilerUtil.h>

CompileStatement::CompileStatement(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, CompilerContext &compiler_context) : module(module), builder(builder), context(context), compiler_context(compiler_context)
{
}

void CompileStatement::Visit(ASTNode *node)
{
}

void CompileStatement::Visit(FunctionNode *node)
{
    auto func = compiler_context.get_function(node->f->FunctionName).function;
    llvm::BasicBlock *EntryBlock = llvm::BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(EntryBlock);
    for (int i = 0; i < node->stmnts.size(); i++)
    {

        node->stmnts[i]->Accept(this);
    }
}

void CompileStatement::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->functions.size(); i++)
    {
        node->functions[i]->Accept(this);
    }
}

void CompileStatement::Visit(ReturnNode *node)
{
    CompileExpr c(module, builder, context, compiler_context);
    builder.CreateRet(c.Expression(node->Expr));
}

void CompileStatement::Visit(ProgramNode *node)
{
    for (const auto &[key, current_module] : node->avail_modules)
    {
        current_module->Accept(this);
        // std::cout << "Key: " << key << ", Value: " << value << std::endl;
    }
    // for (int i = 0; i < node->modules.size(); i++)
    // {
    //     node->modules[i]->Accept(this);
    // }
}

void CompileStatement::Visit(FunctionCallNode *node)
{
    builder.CreateCall(compiler_context.get_function(node->name).function, {});
}
