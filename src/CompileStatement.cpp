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
    // Create the entry block for the function
    // auto func = func_map[node->f->FunctionName.value].function;
    auto func = compiler_context.get_function(node->f->FunctionName).function;
    llvm::BasicBlock *EntryBlock = llvm::BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(EntryBlock);
    // DEBUG ONLY
    //  if (node->f->FunctionName.value != "hello_world")
    //  builder.CreateCall(compiler_context.get_function({"hello_world", TokenType::Identifier, 0}).function, {});
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