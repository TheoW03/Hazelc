#include <backend/compiler_visitors.h>
#include <backend/CompilerUtil.h>
#include <backend/CompilerContext.h>

CompileStatement::CompileStatement(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context,
                                   CompilerContext compiler_context) : module(module), builder(builder), context(context)
{
    this->compiler_context = compiler_context;
    this->program_scope = compiler_context.getScope();
}

void CompileStatement::Visit(ASTNode *node)
{
}

void CompileStatement::Visit(FunctionNode *node)
{
    // auto c = this->program_scope.get_current_function();
    auto c = this->program_scope.set_current_function();
    if (this->program_scope.get_global_function(c.name).has_value())
    {
        auto func = program_scope.get_function(node->f->FunctionName);
        llvm::BasicBlock *EntryBlock = llvm::BasicBlock::Create(context, "entry", func.function);
        builder.SetInsertPoint(EntryBlock);
    }
    else
    {
        program_scope.addLocal(c.name, c);
        llvm::BasicBlock *EntryBlock = llvm::BasicBlock::Create(context, "entry", c.function);
        builder.SetInsertPoint(EntryBlock);
    }
    for (int i = 0; i < node->stmnts.size(); i++)
    {
        node->stmnts[i]->Accept(this);
    }
    // Function c = compiler_context.get_current_module().functions.front();
    // compiler_context.get_current_module().functions.pop();
    // std::cout << compiler_context.get_current_module().functions.size() << std::endl;
    // compiler_context.get_current_module().functions.erase(compiler_context.get_current_module().functions.begin());
    // std::cout << "from data structure: " << c.name.value << std::endl;
    // std::cout << "from AST: " << node->f->FunctionName.value << std::endl;
    // std::cout << "from AST: " << node->f->FunctionName.value << std::endl;
}

void CompileStatement::Visit(ModuleNode *node)
{

    for (int i = 0; i < node->functions.size(); i++)
    {
        // program_scope.set_current(node->name);
        node->functions[i]->Accept(this);
    }
}

void CompileStatement::Visit(ReturnNode *node)
{
    CompileExpr c(module, builder, context, compiler_context, this->program_scope);
    builder.CreateRet(c.Expression(node->Expr));
}

void CompileStatement::Visit(ProgramNode *node)
{
    for (const auto &[key, current_module] : node->avail_modules)
    {
        program_scope.set_current(current_module->name);
        // std::cout << "working in program node " << std::endl;
        // std::cout << current_module->name.value << std::endl;
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
    builder.CreateCall(this->program_scope.get_function(node->name).function, {});
}
