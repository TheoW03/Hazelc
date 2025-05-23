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
    if (!modules.get_global_function(node->f->FunctionName).has_value())
    {
        if (this->local_functions.find(node->f->FunctionName.value) != this->local_functions.end())
        {
            this->local_functions[node->f->FunctionName.value] = node->f;
        }
        else
        {
            this->local_functions.insert(std::make_pair(node->f->FunctionName.value, node->f));
        }
    }
    for (int i = 0; i < node->f->params.size(); i++)
    {
        if (this->local_functions.find(node->f->FunctionName.value) != this->local_functions.end())
        {
            this->local_functions[node->f->params[i]->FunctionName.value] = node->f;
        }
        else
        {
            this->local_functions.insert(std::make_pair(node->f->params[i]->FunctionName.value, node->f->params[i]));
        }
    }
    for (int i = 0; i < node->stmnts->functions.size(); i++)
    {
        node->stmnts->functions[i]->Accept(this);
    }
    auto e = node->stmnts->exit->Expr;
    e->Accept(this);
    auto c = CheckExpressionType(node->f->RetType, modules, local_functions);
    auto a = c.traverse_type(e);

    if (!node->f->RetType->can_accept(a.get()))
    {
        std::cout << "hazelc: type error in function " << node->f->FunctionName.value << " expression type doesnt match the return type" << std::endl;
        exit(EXIT_FAILURE);
    }
}
void TypeCheckerVistor::Visit(ExprNode *node)
{
    node->lhs->Accept(this);

    node->rhs->Accept(this);
}
void TypeCheckerVistor::Visit(BooleanExprNode *node)
{
    node->lhs->Accept(this);

    node->rhs->Accept(this);
}

void TypeCheckerVistor::Visit(ConditionalNode *node)
{
    for (int i = 0; i < node->branches.size(); i++)
    {
        auto c = CheckExpressionType(std::make_shared<BoolType>(), modules, local_functions);
        auto a = c.traverse_type(node->branches[i]->condition);
        if (!std::make_shared<BoolType>()->can_accept(a.get()))
        {
            std::cout << "hazelc: conditionals require a boolean expression" << std::endl;
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < node->branches[i]->stmnts->functions.size(); i++)
        {
            node->branches[i]->stmnts->functions[i]->Accept(this);
        }
        auto e = node->branches[i]->stmnts->exit->Expr;
        auto type = CheckExpressionType(node->type, modules, local_functions).traverse_type(e);
        node->branches[i]->stmnts->exit->Expr->Accept(this);
        if (!node->type->can_accept(type.get()))
        {
            std::cout << "hazelc: type error in conditional" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}
CheckExpressionType::CheckExpressionType(std::shared_ptr<Type> match_type, IntermediateScope s, std::map<std::string, std::shared_ptr<FunctionRefNode>> local_functions)
{
    this->match_type = match_type;
    this->local_functions = local_functions;
    this->s = s;
}
std::shared_ptr<Type> CheckExpressionType::traverse_type(std::shared_ptr<ASTNode> expr)
{
    if (dynamic_cast<IntegerNode *>(expr.get()) != nullptr)
    {
        return std::make_shared<IntegerType>(false);
    }
    else if (dynamic_cast<BooleanConstNode *>(expr.get()) != nullptr)
    {
        return std::make_shared<BoolType>();
    }
    else if (dynamic_cast<StringNode *>(expr.get()) != nullptr)
    {
        return std::make_shared<StringType>();
    }
    else if (dynamic_cast<ConditionalNode *>(expr.get()) != nullptr)
    {
        auto f = dynamic_cast<ConditionalNode *>(expr.get());
        for (int i = 0; i < f->branches.size(); i++)
        {
            auto c = CheckExpressionType(std::make_shared<BoolType>(), s, local_functions);
            auto a = c.traverse_type(f->branches[i]->condition);
            if (!std::make_shared<BoolType>()->can_accept(a.get()))
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
        return this->s.get_global_function(f->name).value_or(this->local_functions[f->name.value])->RetType;
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
        return std::make_shared<BoolType>();
    }
    return nullptr;
}