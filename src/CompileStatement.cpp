#include <visitor.h>
#include <backend/CompilerUtil.h>

CompileStatement::CompileStatement(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::map<std::string, Function> func_map) : module(module), builder(builder), context(context)
{
    this->func_map = func_map;
}

void CompileStatement::Visit(ASTNode *node)
{
}

void CompileStatement::Visit(FunctionNode *node)
{
    // Create the entry block for the function
    auto func = func_map[node->f->FunctionName.value].function;
    llvm::BasicBlock *EntryBlock = llvm::BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(EntryBlock);
    for (int i = 0; i < node->stmnts.size(); i++)
    {

        node->stmnts[i]->Accept(this);
    }
}

void CompileStatement::Visit(ReturnNode *node)
{
    CompileExpr c(module, builder, context, func_map);
    builder.CreateRet(c.Expression(node->Expr));
}