#include <visitor.h>
#include <backend/CompilerUtil.h>
Visitor::Visitor()
{
}

Visitor::~Visitor()
{
}
void Visitor::Visit(ASTNode *node)
{
}
void Visitor::Visit(FunctionNode *node)
{
}

CompileHighLevel::CompileHighLevel(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context) : module(module), builder(builder), context(context)
{
    // this->module = module;
    // this->builder = builder;
    // this->context = context;
}

void CompileHighLevel::Visit(ASTNode *node)
{
}
void CompileHighLevel::Visit(FunctionNode *node)
{
    using std::make_pair; // here bc im lazy you may use using on the stack level. but lets stick to this
    auto c = node->f->RetType;
    llvm::FunctionType *functype = llvm::FunctionType::get(
        compileType(builder, c), {}, false);

    llvm::Function *function = llvm::Function::Create(
        functype, llvm::Function::ExternalLinkage, node->f->FunctionName.value, module);
    func_map.insert(make_pair(node->f->FunctionName.value, function));
}

std::map<std::string, llvm::Function *> CompileHighLevel::get_map()
{
    return this->func_map;
}
