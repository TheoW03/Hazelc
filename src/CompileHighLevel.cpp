#include <backend/compiler_visitors.h>
#include <backend/CompilerUtil.h>

// hazel first compiles all functions and headers
// so it seperates the functions out of the function nodes and compiles all functions and imports
// it creates a data strcuture, in ProgramScope
CompileHighLevel::CompileHighLevel(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context) : module(module), builder(builder), context(context)
{
    // std::map<TokenType, OptionalType>
    //     types;
    // types.insert(std::make_pair(TokenType::Integer, OptionalType()));
    // std::map<TokenType, OptionalType> NativeTypes;

    // auto string_type = llvm::StructType::create(context, "string");
    // std::vector<llvm::Type *> elements = {builder.getPtrTy(), builder.getInt64Ty()};
    // string_type->setBody(elements);
    // NativeTypes.insert(std::make_pair(TokenType::Integer, OptionalType(context, builder, builder.getInt64Ty())));
    // NativeTypes.insert(std::make_pair(TokenType::Uinteger, OptionalType(context, builder, builder.getInt64Ty())));

    // NativeTypes.insert(std::make_pair(TokenType::Decimal, OptionalType(context, builder, builder.getDoubleTy())));
    // NativeTypes.insert(std::make_pair(TokenType::boolean, OptionalType(context, builder, builder.getInt1Ty())));

    // NativeTypes.insert(std::make_pair(TokenType::character, OptionalType(context, builder, builder.getInt8Ty())));
    // NativeTypes.insert(std::make_pair(TokenType::Byte, OptionalType(context, builder, builder.getInt8Ty())));
    // NativeTypes.insert(std::make_pair(TokenType::Ubyte, OptionalType(context, builder, builder.getInt8Ty())));

    // NativeTypes.insert(std::make_pair(TokenType::string, OptionalType(context, builder, string_type)));

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
    // We are indexing functions via a hash
    // I do plan to build on top of it.
    // so this may be temporary
    using std::make_pair;
    std::vector<std::shared_ptr<ASTNode>> filter_functions;
    Function compiled_function = CompileFunctionHeader(node->f);
    compiled_function.isAnonymous = node->anonyomous;

    this->compiler_context.add_function(node, compiled_function);
}

void CompileHighLevel::Visit(BranchNode *node)
{
    std::vector<std::shared_ptr<ASTNode>> filter_functions;
    // node->stmnts->Accept(this);
    // for (int i = 0; i < node->stmnts.size(); i++)
    // {
    // node->stmnts[i]->Accept(this);
    // if (dynamic_cast<FunctionNode *>(node->stmnts[i].get()))
    // {
    //     this->functions.push_back(node->stmnts[i]);
    // }
    // else
    // {
    //     filter_functions.push_back(node->stmnts[i]);
    // }
    // }
    // node->stmnts = filter_functions;
}

void CompileHighLevel::Visit(ConditionalNode *node)
{
    for (int i = 0; i < node->branches.size(); i++)
    {
        node->branches[i]->Accept(this);
    }
}
void CompileHighLevel::Visit(ReturnNode *node)
{
    node->Expr->Accept(this);
}

void CompileHighLevel::Visit(FunctionCallNode *node)
{
    // std::cout << node->name.value << std::endl;
    // std::cout << node->param_types.size() << std::endl;

    for (int i = 0; i < node->param_types.size(); i++)
    {
        auto compiled_function = CompileFunctionHeader(node->param_types[i]);
        compiled_function.isAnonymous = true;
        this->compiled_functions.push(compiled_function);
    }
}
void CompileHighLevel::Visit(DemoduarlizedProgramNode *node)
{
    for (int i = 0; i < node->functions.size(); i++)
    {
        node->functions[i]->Accept(this);
    }

    // node->functions = this->functions;
}
// void CompileHighLevel::Visit(ProgramNode *node)
// {
//     for (const auto &[key, current_module] : node->avail_modules)
//     {
//         current_module->Accept(this);
//         compiler_context.AddModule(current_module->name.value, {func_map,
//                                                                 this->exported_func_map,
//                                                                 current_module->imports,
//                                                                 this->compiled_functions});
//         this->func_map.clear();
//         this->exported_func_map.clear();
//         this->compiled_functions = {};
//     }
// }

void CompileHighLevel::Visit(ExprNode *node)
{
    node->lhs->Accept(this);
    node->rhs->Accept(this);
}
void CompileHighLevel::Visit(BlockNode *node)
{
    // for (int i = 0; i < node->functions.size(); i++)
    // {
    //     node->functions[i]->Accept(this);
    //     // this->functions.push_back(node->functions[i]);
    // }
    node->exit->Accept(this);
}

Function CompileHighLevel::CompileFunctionHeader(std::shared_ptr<FunctionRefNode> n)
{
    auto c = n->RetType;

    std::vector<Function> f;
    std::vector<llvm::Type *> a;
    // for (int i = 0; i < n->params.size(); i++)
    // {
    //     auto c = CompileFunctionHeader(n->params[i]);
    //     f.push_back(c);
    //     a.push_back(c.function->getType());
    // }
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

ProgramScope CompileHighLevel::getProgramScope()
{
    return ProgramScope(this->func_map, this->compiled_functions);
}
std::tuple<llvm::FunctionType *, std::vector<Thunks>> CompileHighLevel::compile_Function_Type(std::shared_ptr<FunctionRefNode> n)
{
    auto c = n->RetType;
    std::vector<Thunks> thunks;
    for (int i = 0; i < n->params.size(); i++)
    {
        auto funct = compile_Function_Type(n->params[i]);
        auto p = get_thunk_types(n->params[i]);
        p.gep_loc = params_struct.size() + 1;
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
Thunks CompileHighLevel::get_thunk_types(std::shared_ptr<FunctionRefNode> n)
{
    // std::vector<Thunks> thunks;
    // this->string_type = llvm::StructType::create(context, "Thunk");
    auto thunk = llvm::StructType::create(context, "Thunk");
    auto funct = compile_Function_Type(n);

    std::vector<llvm::Type *> elements = {compiler_context.compile_Type_Optional(n->RetType).type,
                                          llvm::PointerType::getUnqual(std::get<0>(funct)), builder.getInt1Ty()};
    thunk->setBody(elements);
    return {thunk, nullptr};
}
