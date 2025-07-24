#include <backend/compiler_visitors.h>
#include <backend/CompilerUtil.h>
// hazel first compiles all functions and headers
// so it seperates the functions out of the function nodes and compiles all functions and imports
// it creates a data strcuture, in ProgramScope
CompileHighLevel::CompileHighLevel(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context) : module(module), builder(builder), context(context)
{

    this->params = llvm::StructType::create(context, "params");
    CRunTimeFunctions c(builder, module);
    this->compiler_context = CompilerContext(c, context, module, builder);
    // builder.CreateMemCpy()
}

void CompileHighLevel::Visit(ASTNode *node)
{
}
void CompileHighLevel::Visit(FunctionNode *node)
{

    // we compile the function Node
    //  then put it in a map if its global and a stack
    Function compiled_function = CompileFunctionHeader(node->f);
    compiled_function.isAnonymous = node->anonyomous;
    this->compiler_context.add_function(node, compiled_function);
}

void CompileHighLevel::Visit(DemoduarlizedProgramNode *node)
{
    for (int i = 0; i < node->functions.size(); i++)
    {
        node->functions[i]->Accept(this);
    }
    std::cout << "hazelc: Compiled Functions" << std::endl;

    // node->functions = this->functions;
}

Function CompileHighLevel::CompileFunctionHeader(std::shared_ptr<FunctionRefNode> n)
{
    auto c = n->RetType;

    std::vector<Function> f;
    std::vector<llvm::Type *> a;
    auto functype = this->compile_Function_Type(n);
    llvm::Function *function = llvm::Function::Create(
        std::get<0>(functype), llvm::Function::ExternalLinkage, n->FunctionName.value, module);
    auto retty = compiler_context.compile_Type_Optional(c).type;

    // to be more safer I end up using sret for return types.
    // since all types in hazel ae infact functions. sret makes the most sense
    function->getArg(1)->addAttr(llvm::Attribute::getWithStructRetType(context, retty));
    function->getArg(1)->setName("ret");

    return {function, f, n->RetType, n->FunctionName, std::get<1>(functype), false};
}

std::tuple<llvm::FunctionType *, std::vector<FunctionParam>> CompileHighLevel::compile_Function_Type(std::shared_ptr<FunctionRefNode> n)
{
    auto c = n->RetType;
    std::vector<FunctionParam> thunks;
    for (int i = 0; i < n->params.size(); i++)
    {
        auto funct = compile_Function_Type(n->params[i]);
        auto p = get_thunk_types(n->params[i]);
        p.gep_loc = params_struct.size();
        params_struct.push_back(p.thunk_type);
        thunks.push_back(p);

        // this->params->
        // a.push_back(get_thunk_types(builder, context, n->params[i]).thunk_type);
    }
    auto retty = compiler_context.compile_Type_Optional(c).type;
    llvm::FunctionType *functype = llvm::FunctionType::get(
        builder.getVoidTy(),
        {
            llvm::PointerType::get(params, 0),
            llvm::PointerType::get(retty, 0),
        },
        false);

    return {functype, thunks};
}
FunctionParam CompileHighLevel::get_thunk_types(std::shared_ptr<FunctionRefNode> n)
{
    auto thunk = llvm::StructType::create(context, "Thunk");
    auto funct = compile_Function_Type(n);

    std::vector<llvm::Type *> elements = {compiler_context.compile_Type_Optional(n->RetType).type,
                                          llvm::PointerType::getUnqual(std::get<0>(funct)), builder.getInt1Ty(), llvm::PointerType::getUnqual(this->params)};
    thunk->setBody(elements);
    return {n->FunctionName, thunk, std::get<0>(funct), n->RetType, 0};
}
