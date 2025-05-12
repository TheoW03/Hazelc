#include <vector>
#include <Frontend/SemanticCheckScopes.h>

TypeCheckerVistor::TypeCheckerVistor(std::map<std::string, SemanticModule> modules)
{
    this->modules = modules;
}
void TypeCheckerVistor::Visit(ProgramNode *node)
{
    for (const auto &pair : node->avail_modules)
    {
        pair.second->Accept(this);
    }
}

void TypeCheckerVistor::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->functions.size(); i++)
    {
        node->functions[i]->Accept(this);
    }
}
void TypeCheckerVistor::Visit(FunctionNode *node)
{
    auto e = node->stmnts->exit->Expr;
    auto c = CheckExpressionType(node->f->RetType);
    auto a = c.traverse_type(e);
    if (!node->f->RetType->can_accept(a.get()))
    {
        std::cout << "hazelc: type error in function " << node->f->FunctionName.value << " expression type doesnt match the return type" << std::endl;
        exit(EXIT_FAILURE);
    }
}
CheckExpressionType::CheckExpressionType(std::shared_ptr<Type> match_type)
{
    this->match_type = match_type;
}
std::shared_ptr<Type> CheckExpressionType::traverse_type(std::shared_ptr<ASTNode> expr)
{
    if (dynamic_cast<IntegerNode *>(expr.get()) != nullptr)
    {
        return std::make_shared<NativeType>(TokenType::Integer);
    }
    return std::make_shared<NativeType>(TokenType::Decimal);
}