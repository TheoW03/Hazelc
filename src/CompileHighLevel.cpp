#include <visitor.h>
#include <backend/CompilerUtil.h>

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
        this->func_map.insert(std::make_pair(node->f->FunctionName.value, compiled_function));
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

void CompileHighLevel::Visit(ReturnNode *node)
{
}

void CompileHighLevel::Visit(FunctionCallNode *node)
{
}

void CompileHighLevel::Visit(ProgramNode *node)
{

    for (const auto &[key, current_module] : node->avail_modules)
    {
        current_module->Accept(this);
        compiler_context.AddModule(current_module->name.value, {func_map, current_module->imports, this->compiled_functions});
        this->func_map.clear();
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

    return {function, f, n->RetType, n->FunctionName};
}
