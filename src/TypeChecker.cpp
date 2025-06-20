#include <vector>
#include <Frontend/SemanticCheckScopes.h>
#include <error.h>
TypeCheckerVistor::TypeCheckerVistor(IntermediateScope modules)
{
    this->modules = modules;
}
// void TypeCheckerVistor::Visit(ProgramNode *node)
// {
//     for (const auto &pair : node->avail_modules)
//     {
//         // this->current_AST_module = this->modules.modules[pair.first];
//         modules.set_current_module(pair.first);
//         pair.second->Accept(this);
//     }
// }

// void TypeCheckerVistor::Visit(ModuleNode *node)
// {
//     for (int i = 0; i < node->functions.size(); i++)
//     {
//         node->functions[i]->Accept(this);
//     }
// }
void TypeCheckerVistor::add_local_function(Tokens name, std::shared_ptr<FunctionRefNode> value)
{
    if (this->local_functions.find(name.value) != this->local_functions.end())
    {
        this->local_functions[name.value] = value;
    }
    else
    {
        this->local_functions.insert(std::make_pair(name.value, value));
    }
}
void TypeCheckerVistor::Visit(FunctionNode *node)
{
    if (!node->hash_name.has_value())
    {
        add_local_function(node->f->FunctionName, node->f);
    }
    for (int i = 0; i < node->f->params.size(); i++)
    {
        add_local_function(node->f->FunctionName, node->f);
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
        error("type error in function, expects a " + node->f->RetType->get_type_value() + " got a " + a->get_type_value(), node->f->FunctionName);
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
void TypeCheckerVistor::Visit(DemoduarlizedProgramNode *node)
{
    for (const auto &[key, current_function] : node->global_functions)
    {

        current_function->Accept(this);
    }
}
void TypeCheckerVistor::Visit(ConditionalNode *node)
{
    for (int i = 0; i < node->branches.size(); i++)
    {
        auto c = CheckExpressionType(std::make_shared<BoolType>(), modules, local_functions);
        auto a = c.traverse_type(node->branches[i]->condition);
        if (!std::make_shared<BoolType>()->can_accept(a.get()))
        {
            error("expected boolean got " + a->get_type_value(), node->node);
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
            error("expected " + node->type->get_type_value() + " type got " + type->get_type_value() + " type", node->node);
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
    else if (dynamic_cast<DecimalNode *>(expr.get()) != nullptr)
    {
        return std::make_shared<DecimalType>();
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
        return f->type;
    }
    else if (dynamic_cast<FunctionCallNode *>(expr.get()) != nullptr)
    {
        auto f = dynamic_cast<FunctionCallNode *>(expr.get());
        return this->s.get_global_function_demodularized(f->hash_name).value_or(this->local_functions[f->name.value])->RetType;
    }
    else if (dynamic_cast<ExprNode *>(expr.get()) != nullptr)
    {
        auto exprs = dynamic_cast<ExprNode *>(expr.get());
        auto lhstype = traverse_type(exprs->lhs);
        auto rhstype = traverse_type(exprs->rhs);
        if (!lhstype->can_accept(rhstype.get()))
        {
            error("expected " + lhstype->get_type_value() + " got " + rhstype->get_type_value(), exprs->operation);
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
            error("expected " + lhstype->get_type_value() + "got " + rhstype->get_type_value(), exprs->op);
        }
        return std::make_shared<BoolType>();
    }
    return nullptr;
}