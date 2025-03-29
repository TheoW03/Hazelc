#include <Frontend/Ast.h>
#include <visitor.h>
#include <map>

ASTNode::~ASTNode()
{
}
ASTNode::ASTNode()
{
}
IntegerNode::IntegerNode(Tokens num)
{
    if (num.type == TokenType::BaseTenDigit)
    {
        this->number = stoi(num.value);
    }
    else if (num.type == TokenType::HexDigit)
    {
        this->number = std::stoul(num.value, nullptr, 16);
    }
    else if (num.type == TokenType::BinaryDigit)
    {
        this->number = std::stoul(num.value, nullptr, 2);
    }
}
IntegerNode::IntegerNode(int num)
{
    this->number = num;
}
void IntegerNode::Accept(Visitor *v) {}

std::string IntegerNode::to_string()
{
    return std::to_string(this->number);
}
ExprNode::ExprNode(std::shared_ptr<ASTNode> lhs, Tokens operation, std::shared_ptr<ASTNode> rhs)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->operation = operation;
}

std::shared_ptr<ASTNode> ExprNode::fold()
{
    return std::shared_ptr<ASTNode>();
}
void ExprNode::Accept(Visitor *v)
{
}

std::string ExprNode::to_string()
{
    std::map<TokenType, std::string> token_map;

    token_map[TokenType::Addition] = "+";
    token_map[TokenType::Subtraction] = "-";
    token_map[TokenType::Multiplication] = "*";
    token_map[TokenType::Division] = "/";
    token_map[TokenType::Left_Shift] = "<<";

    return lhs->to_string() + token_map[operation.type] + rhs->to_string();
}

FunctionRefNode::FunctionRefNode(Tokens name,
                                 std::vector<std::shared_ptr<FunctionRefNode>> params,
                                 std::shared_ptr<Type> returnType)
{

    this->FunctionName = name;
    // this->can_export
    this->RetType = returnType;
    this->params = params;
}

void FunctionRefNode::Accept(Visitor *v)
{
    // std::shared_ptr<FunctionNode> sharedPtr(this);
    v->Visit(this);
    // v.Visit(std::make_shared<FunctionNode>(this));
}

std::string FunctionRefNode::to_string()
{
    return std::string();
}

FunctionNode::FunctionNode(bool can_export, std::shared_ptr<FunctionRefNode> functionHeader, std::vector<std::shared_ptr<ASTNode>> stmnts)
{

    this->can_export = can_export;
    this->f = functionHeader;
    this->stmnts = stmnts;
}

void FunctionNode::Accept(Visitor *v)
{
    v->Visit(this);
    // v.Visit(std::make_shared<FunctionNode>(this));
}

std::string FunctionNode::to_string()
{
    return std::string();
}

std::string NativeType::to_string()
{
    return std::string();
}
Type::Type() {}
Type::~Type() {}
FunctionType::FunctionType(std::vector<std::shared_ptr<Type>> params,
                           std::shared_ptr<Type> returnType)
{
}
std::string FunctionType::get_type_value()
{
    return std::string();
}
NativeType::NativeType(Tokens type)
{
    this->type = type;
}

std::string NativeType::get_type_value()
{
    return type.value;
}
std::string FunctionType::to_string()
{
    return std::string();
}

std::string Type::to_string()
{
    return "";
}

NoneNode::NoneNode()
{
}

void NoneNode::Accept(Visitor *v)
{
    v->Visit(this);
}

std::string NoneNode::to_string()
{
    return std::string();
}

ReturnNode::ReturnNode(std::shared_ptr<ASTNode> expr)
{
    this->Expr = expr;
}

void ReturnNode::Accept(Visitor *v)
{
    v->Visit(this);
}

std::string ReturnNode::to_string()
{
    return std::string();
}

ListType::ListType(std::shared_ptr<Type> inner)
{
    this->inner_type = inner;
}

std::string ListType::get_type_value()
{
    return inner_type->get_type_value();
}

std::string ListType::to_string()
{
    return std::string();
}

ModuleNode::ModuleNode(std::vector<std::shared_ptr<ASTNode>> functions, Tokens name, std::vector<Tokens> imports)
{
    this->functions = functions;
    this->name = name;
    this->imports = imports;
}

void ModuleNode::Accept(Visitor *v)
{
    v->Visit(this);
}

std::string ModuleNode::to_string()
{
    return std::string();
}

DecimalNode::DecimalNode(Tokens num)
{
    this->number = std::stod(num.value);
}

DecimalNode::DecimalNode(double num)
{
    this->number = num;
}
void DecimalNode::Accept(Visitor *v)
{
}

std::string DecimalNode::to_string()
{
    return std::string();
}

BooleanExprNode::BooleanExprNode(std::shared_ptr<ASTNode> lhs, Tokens operation, std::shared_ptr<ASTNode> rhs)
{
    this->lhs = lhs;
    this->rhs = rhs;
    this->op = operation;
}

void BooleanExprNode::Accept(Visitor *v)
{
}

std::string BooleanExprNode::to_string()
{
    return std::string();
}

BooleanConstNode::BooleanConstNode(Tokens value)
{
    this->value = value;
}

void BooleanConstNode::Accept(Visitor *v)
{
}

std::string BooleanConstNode::to_string()
{
    return std::string();
}

ListNode::ListNode(std::vector<std::shared_ptr<ASTNode>> values)
{
    this->values = values;
}

void ListNode::Accept(Visitor *v)
{
}

std::string ListNode::to_string()
{
    return std::string();
}

StringNode::StringNode(Tokens value)
{
    this->value = value.value;
}
StringNode::StringNode(std::string value)
{
    this->value = value;
}

void StringNode::Accept(Visitor *v)
{
}

std::string StringNode::to_string()
{
    return std::string();
}

CharNode::CharNode(Tokens value)
{
    this->value = value;
}

void CharNode::Accept(Visitor *v)
{
}

std::string CharNode::to_string()
{
    return std::string();
}

FunctionCallNode::FunctionCallNode(Tokens name, std::vector<std::shared_ptr<ASTNode>> params)
{
    this->name = name;
    this->params = params;
}

void FunctionCallNode::Accept(Visitor *v)
{
    v->Visit(this);
}

std::string FunctionCallNode::to_string()
{
    return std::string();
}

ProgramNode::ProgramNode(std::map<std::string, std::shared_ptr<ModuleNode>> modules)
{

    this->avail_modules = modules;
}

void ProgramNode::Accept(Visitor *v)
{
    v->Visit(this);
}

std::string ProgramNode::to_string()
{
    return std::string();
}
