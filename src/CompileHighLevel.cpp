#include <visitor.h>
#include <backend/CompilerUtil.h>

CompileHighLevel::CompileHighLevel(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context) : module(module), builder(builder), context(context)
{
    // this->module = module;
    // this->builder = builder;
    // this->context = context;
    this->compiler_context.compile_cfunctions(module, context, builder);
}

void CompileHighLevel::Visit(ASTNode *node)
{
}
void CompileHighLevel::Visit(FunctionNode *node)
{
    using std::make_pair;
    // auto c = node->f->RetType;
    // llvm::FunctionType *functype = llvm::FunctionType::get(
    //     compileType(builder, c), {}, false);

    // llvm::Function *function = llvm::Function::Create(
    //     functype, llvm::Function::ExternalLinkage, node->f->FunctionName.value, module);
    // func_map.insert(, node->f->FunctionName.value, );
    std::vector<std::shared_ptr<ASTNode>> filter_functions;

    compiler_context.add_function(node->f->FunctionName, CompileFunctionHeader(node->f));

    for (int i = 0; i < node->stmnts.size(); i++)
    {

        node->stmnts[i]->Accept(this);
        if (dynamic_cast<FunctionNode *>(node->stmnts[i].get()))
        {
            functions.push_back(node->stmnts[i]);
        }
        else
        {
            filter_functions.push_back(node->stmnts[i]);
        }
    }
    node->stmnts = filter_functions;
}

void CompileHighLevel::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->functions.size(); i++)
    {
        functions.push_back(node->functions[i]);

        node->functions[i]->Accept(this);
    }
}

void CompileHighLevel::Visit(ReturnNode *node)
{
}

void CompileHighLevel::Visit(FunctionCallNode *node)
{
}

Function CompileHighLevel::CompileFunctionHeader(std::shared_ptr<FunctionRefNode> n)
{
    auto c = n->RetType;

    std::vector<Function> f;
    // std::vector<llvm::Type *> a;
    // for (int i = 0; i < n->params.size(); i++)
    // {
    //     auto c = CompileFunctionHeader(n->params[i]);
    //     f.push_back(c);
    //     a.push_back(c.function->getType());
    // }
    llvm::FunctionType *functype = this->compiler_context.compile_Function_Type(builder, context, n);
    llvm::Function *function = llvm::Function::Create(
        functype, llvm::Function::ExternalLinkage, n->FunctionName.value, module);

    return {function, f};
}
