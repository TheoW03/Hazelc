#include <vector>
#include <Frontend/SemanticCheckScopes.h>

TypeCheckerVistor::TypeCheckerVistor(IntermediateScope modules)
{
    this->modules = modules;
}
void TypeCheckerVistor::Visit(ProgramNode *node)
{
    for (const auto &pair : node->avail_modules)
    {
        // this->current_AST_module = this->modules.modules[pair.first];
        modules.set_current_module(pair.first);
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
    auto c = CheckExpressionType(node->f->RetType, modules);
    auto a = c.traverse_type(e);
    if (!node->f->RetType->can_accept(a.get()))
    {
        std::cout << "hazelc: type error in function " << node->f->FunctionName.value << " expression type doesnt match the return type" << std::endl;
        exit(EXIT_FAILURE);
    }
}
CheckExpressionType::CheckExpressionType(std::shared_ptr<Type> match_type, IntermediateScope s)
{
    this->match_type = match_type;
    this->s = s;
}
std::shared_ptr<Type> CheckExpressionType::traverse_type(std::shared_ptr<ASTNode> expr)
{
    if (dynamic_cast<IntegerNode *>(expr.get()) != nullptr)
    {
        return std::make_shared<NativeType>(TokenType::Integer);
    }
    else if (dynamic_cast<BooleanConstNode *>(expr.get()) != nullptr)
    {
        return std::make_shared<NativeType>(TokenType::boolean);
    }
    else if (dynamic_cast<StringNode *>(expr.get()) != nullptr)
    {
        return std::make_shared<NativeType>(TokenType::string);
    }
    else if (dynamic_cast<ConditionalNode *>(expr.get()) != nullptr)
    {
        auto f = dynamic_cast<ConditionalNode *>(expr.get());
        for (int i = 0; i < f->branches.size(); i++)
        {
            auto c = CheckExpressionType(std::make_shared<NativeType>(TokenType::boolean), s);
            auto a = c.traverse_type(f->branches[i]->condition);
            if (!std::make_shared<NativeType>(TokenType::boolean)->can_accept(a.get()))
            {
                std::cout << "hazelc: conditionals require a boolean expression" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return f->type;
    }
    else if (dynamic_cast<FunctionCallNode *>(expr.get()) != nullptr)
    {
        auto f = dynamic_cast<FunctionCallNode *>(expr.get());
        return this->s.get_global_function(f->name).value()->f->RetType;
    }
    else if (dynamic_cast<ExprNode *>(expr.get()) != nullptr)
    {
        auto exprs = dynamic_cast<ExprNode *>(expr.get());
        auto lhstype = traverse_type(exprs->lhs);
        auto rhstype = traverse_type(exprs->rhs);
        if (!lhstype->can_accept(rhstype.get()))
        {
            std::cout << "hazelc: type error in expr" << std::endl;
            exit(EXIT_FAILURE);
        }
        return rhstype;
    }
    else if (dynamic_cast<BooleanExprNode *>(expr.get()) != nullptr)
    {
        auto exprs = dynamic_cast<BooleanExprNode *>(expr.get());
        auto lhstype = traverse_type(exprs->lhs);
        auto rhstype = traverse_type(exprs->rhs);
        if (!lhstype->can_accept(rhstype.get()))
        {
            std::cout << "hazelc: type error in expr" << std::endl;
            exit(EXIT_FAILURE);
        }
        return std::make_shared<NativeType>(TokenType::boolean);
    }
    return nullptr;
}