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
    using std::make_pair;
    // auto c = node->f->RetType;
    // llvm::FunctionType *functype = llvm::FunctionType::get(
    //     compileType(builder, c), {}, false);

    // llvm::Function *function = llvm::Function::Create(
    //     functype, llvm::Function::ExternalLinkage, node->f->FunctionName.value, module);
    // func_map.insert(, node->f->FunctionName.value, );
    func_map.insert(make_pair(node->f->FunctionName.value, CompileFunctionHeader(node->f)));
}

void CompileHighLevel::Visit(ReturnNode *node)
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
    llvm::FunctionType *functype = CompileFunctionType(builder, n);
    llvm::Function *function = llvm::Function::Create(
        functype, llvm::Function::ExternalLinkage, n->FunctionName.value, module);
    return {function, f};
}

std::map<std::string, Function> CompileHighLevel::get_map()
{
    return this->func_map;
}

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

CompileExpr::CompileExpr(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context, std::map<std::string, Function> func_map) : module(module), builder(builder), context(context)
{
    this->func_map = func_map;
}

llvm::Value *CompileExpr::Expression(std::shared_ptr<ASTNode> node)
{
    if (dynamic_cast<IntegerNode *>(node.get()))
    {
        auto c = dynamic_cast<IntegerNode *>(node.get());
        return llvm::ConstantInt::get(llvm::Type::getInt64Ty(context), c->number);
    }
    else if (dynamic_cast<ExprNode *>(node.get()))
    {
        auto c = dynamic_cast<ExprNode *>(node.get());

        auto lhs = Expression(c->lhs);
        auto rhs = Expression(c->rhs);
        switch (c->operation.type)
        {
        case Addition:
            return builder.CreateAdd(lhs, rhs, "addition");
        default:
            break;
        }
    }
    return nullptr;
}
