#include <backend/compiler_visitors.h>
#include <backend/CompilerUtil.h>

// hazel first compiles all functions and headers
// so it seperates the functions out of the function nodes and compiles all functions and imports
// it creates a data strcuture, in ProgramScope
CompileHighLevel::CompileHighLevel(llvm::Module &module, llvm::IRBuilder<> &builder, llvm::LLVMContext &context) : module(module), builder(builder), context(context)
{
    std::map<std::string, llvm::Function *> CFunctions;
    auto snprintftype = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy(), builder.getInt64Ty(), builder.getInt8PtrTy()}, true);
    auto snprintffunc = llvm::Function::Create(
        snprintftype, llvm::Function::ExternalLinkage, "snprintf", module);
    CFunctions.insert(std::make_pair("snprintf", snprintffunc));
    auto printftype = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy()}, true);
    auto printf_func = llvm::Function::Create(
        printftype, llvm::Function::ExternalLinkage, "printf", module);
    CFunctions.insert(std::make_pair("printf", printf_func));

    auto strncpy_type = llvm::FunctionType::get(builder.getInt32Ty(), {builder.getInt8PtrTy(), builder.getInt8PtrTy(), builder.getInt64Ty()}, false);
    auto strncpy_func = llvm::Function::Create(
        strncpy_type, llvm::Function::ExternalLinkage, "strncpy", module);
    CFunctions.insert(std::make_pair("strncpy", strncpy_func));
    std::map<TokenType, OptionalType> types;
    // types.insert(std::make_pair(TokenType::Integer, OptionalType()));
    std::map<TokenType, OptionalType> NativeTypes;

    auto string_type = llvm::StructType::create(context, "string");
    std::vector<llvm::Type *> elements = {builder.getInt8PtrTy(), builder.getInt64Ty()};

    string_type->setBody(elements);
    NativeTypes.insert(std::make_pair(TokenType::Integer, OptionalType(context, builder, builder.getInt64Ty())));
    NativeTypes.insert(std::make_pair(TokenType::Uinteger, OptionalType(context, builder, builder.getInt64Ty())));

    NativeTypes.insert(std::make_pair(TokenType::Decimal, OptionalType(context, builder, builder.getDoubleTy())));
    NativeTypes.insert(std::make_pair(TokenType::boolean, OptionalType(context, builder, builder.getInt1Ty())));

    NativeTypes.insert(std::make_pair(TokenType::character, OptionalType(context, builder, builder.getInt8Ty())));
    NativeTypes.insert(std::make_pair(TokenType::Byte, OptionalType(context, builder, builder.getInt8Ty())));
    NativeTypes.insert(std::make_pair(TokenType::Ubyte, OptionalType(context, builder, builder.getInt8Ty())));

    NativeTypes.insert(std::make_pair(TokenType::string, OptionalType(context, builder, string_type)));

    this->params = llvm::StructType::create(context, "params");
    this->compiler_context = CompilerContext(CFunctions, NativeTypes, string_type);
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
    Function compiled_function = CompileFunctionHeader(node->f);
    // std::cout << "func name: " << node->f->FunctionName.value << std::endl;
    // compiler_context.add_function(node->f->FunctionName, CompileFunctionHeader(node->f));
    if (is_global)
    {
        this->func_map.insert(std::make_pair(node->f->FunctionName.value, compiled_function));
        if (node->can_export)
            this->exported_func_map.insert(std::make_pair(node->f->FunctionName.value, compiled_function));
    }
    is_global = false;
    this->compiled_functions.push(compiled_function);
    for (int i = 0; i < node->stmnts.size(); i++)
    {

        node->stmnts[i]->Accept(this);
        if (dynamic_cast<FunctionNode *>(node->stmnts[i].get()))
        {
            this->functions.push_back(node->stmnts[i]);
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
        is_global = true;
        node->functions[i]->Accept(this);
    }
    node->functions = functions;
    functions.clear();
}

void CompileHighLevel::Visit(BranchNode *node)
{
    std::vector<std::shared_ptr<ASTNode>> filter_functions;

    for (int i = 0; i < node->stmnts.size(); i++)
    {
        node->stmnts[i]->Accept(this);
        if (dynamic_cast<FunctionNode *>(node->stmnts[i].get()))
        {
            this->functions.push_back(node->stmnts[i]);
        }
        else
        {
            filter_functions.push_back(node->stmnts[i]);
        }
    }
    node->stmnts = filter_functions;
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
}

void CompileHighLevel::Visit(ProgramNode *node)
{

    for (const auto &[key, current_module] : node->avail_modules)
    {
        current_module->Accept(this);
        compiler_context.AddModule(current_module->name.value, {func_map,
                                                                this->exported_func_map,
                                                                current_module->imports,
                                                                this->compiled_functions});
        this->func_map.clear();
        this->exported_func_map.clear();
        this->compiled_functions = {};

        // std::cout << "Key: " << key << ", Value: " << value << std::endl;
    }
    // for (int i = 0; i < node->modules.size(); i++)
    // {
    //     node->modules[i]->Accept(this);
    // }
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

    return {function, f, n->RetType, n->FunctionName, std::get<1>(functype)};
}

ProgramScope CompileHighLevel::getProgramScope()
{
    return ProgramScope(this->modules);
}
std::tuple<llvm::FunctionType *, std::vector<Thunks>> CompileHighLevel::compile_Function_Type(std::shared_ptr<FunctionRefNode> n)
{
    auto c = n->RetType;
    std::vector<Thunks> thunks;
    for (int i = 0; i < n->params.size(); i++)
    {
        auto funct = compile_Function_Type(n->params[i]);
        auto p = get_thunk_types(n->params[i]);
        params_struct.push_back(p.thunk_type);
        thunks.push_back(p);
        // this->params->
        // a.push_back(get_thunk_types(builder, context, n->params[i]).thunk_type);
    }
    llvm::FunctionType *functype = llvm::FunctionType::get(
        compiler_context.compile_Type_Optional(c).type, params, false);
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
