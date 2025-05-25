#include <Frontend/Ast.h>
#include <visitor.h>
#include <map>
#include <error.h>

ASTNode::~ASTNode()
{
}

ASTNode::ASTNode(NodeLocation node)
{
    this->node = node;
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
IntegerNode::IntegerNode(Tokens num, NodeLocation location) : ASTNode(location)
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
void IntegerNode::Accept(Visitor *v)
{
    v->Visit(this);
}

ASTNode::ASTNode()
{
}
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
    v->Visit(this);
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

std::shared_ptr<Type> FunctionRefNode::get_func_type()
{
    std::vector<std::shared_ptr<Type>> param_types;
    for (int i = 0; i < params.size(); i++)
    {
        param_types.push_back(params[i]->get_func_type());
    }
    return std::make_shared<FunctionType>(param_types, RetType);
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

FunctionNode::FunctionNode(bool can_export, std::shared_ptr<FunctionRefNode> functionHeader, std::shared_ptr<BlockNode> stmnts)
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

// std::string NativeType::to_string()
// {
//     return std::string();
// }
Type::Type() {}
Type::~Type() {}

// NativeType::NativeType(Tokens type)
// {
//     this->type = type;
// }

// NativeType::NativeType(TokenType t)
// {
//     this->type = {"", t, 0};
// }

// std::string NativeType::get_type_value()
// {
//     return type.value;
// }
IntegerType::IntegerType(bool is_unsigned)
{
    this->is_unsigned = is_unsigned;
}
std::string IntegerType::get_type_value()
{
    if (is_unsigned)
        return "unsigned integer";
    return "integer";
}
bool IntegerType::can_accept(Type *type)
{
    if (dynamic_cast<IntegerType *>(type))
    {
        auto f = dynamic_cast<IntegerType *>(type);
        return f->is_unsigned == this->is_unsigned;
    }
    return false;
}
ByteType::ByteType(bool is_unsigned)
{
    this->is_unsigned = is_unsigned;
}
std::string ByteType::get_type_value()
{
    if (is_unsigned)
        return "unsigned byte";
    return "byte";
}
bool ByteType::can_accept(Type *type)
{
    if (dynamic_cast<ByteType *>(type))
    {
        auto f = dynamic_cast<ByteType *>(type);
        return f->is_unsigned == this->is_unsigned;
    }
    return false;
}
CharacterType::CharacterType()
{
}

std::string CharacterType::get_type_value()
{
    return "character";
}
bool CharacterType::can_accept(Type *type)
{
    return dynamic_cast<CharacterType *>(type);
}
BoolType::BoolType()
{
}
std::string BoolType::get_type_value()
{
    return "boolean";
}

bool BoolType::can_accept(Type *type)
{
    return dynamic_cast<BoolType *>(type);
}
// bool NativeType::can_accept(Type *type)
// {
//     if (dynamic_cast<NativeType *>(type))
//     {
//         auto c = dynamic_cast<NativeType *>(type);
//         return c->type.type == this->type.type;
//     }
//     return false;
// }
FunctionType::FunctionType(std::vector<std::shared_ptr<Type>> params,
                           std::shared_ptr<Type> returnType)
{
    this->params = params;
    this->returnType = returnType;
}
std::string FunctionType::get_type_value()
{
    return std::string();
}
bool FunctionType::can_accept(Type *type)
{
    if (type->can_accept(returnType.get()))
    {
        for (int i = 0; i < params.size(); i++)
        {
            if (!params[i]->can_accept(type))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

std::string FunctionType::to_string()
{
    return std::string();
}

std::string Type::to_string()
{
    return "";
}
std::string BoolType::to_string()
{
    return "";
}
std::string ByteType::to_string()
{
    return "";
}
std::string CharacterType::to_string()
{
    return "";
}
std::string IntegerType::to_string()
{
    return "";
}
std::string DecimalType::to_string()
{
    return "";
}
std::string StringType::to_string()
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
DecimalType::DecimalType()
{
}
std::string DecimalType::get_type_value()
{
    return "decimal";
}
bool DecimalType::can_accept(Type *type)
{
    return dynamic_cast<DecimalType *>(type);
}

StringType::StringType()
{
}
bool StringType::can_accept(Type *type)
{
    return dynamic_cast<StringType *>(type);
}
std::string StringType::get_type_value()
{
    return "string";
}

ListType::ListType(std::shared_ptr<Type> inner)
{
    this->inner_type = inner;
}

std::string ListType::get_type_value()
{
    return "List inner=" + inner_type->get_type_value();
}
bool ListType::can_accept(Type *type)
{
    return false;
}
std::string ListType::to_string()
{
    return std::string();
}

ModuleNode::ModuleNode(std::vector<std::shared_ptr<FunctionNode>> functions, Tokens name, std::vector<Tokens> imports)
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
    v->Visit(this);
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
    v->Visit(this);
}

std::string BooleanExprNode::to_string()
{
    return std::string();
}

BooleanConstNode::BooleanConstNode(Tokens value)
{
    this->val = (value.type == TokenType::True) ? true : false;
}

BooleanConstNode::BooleanConstNode(bool value)
{
    this->val = value;
}
void BooleanConstNode::Accept(Visitor *v)
{
    v->Visit(this);
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
    v->Visit(this);
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
    v->Visit(this);
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
    v->Visit(this);
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

// Program node is the root node
ProgramNode::ProgramNode(std::map<std::string, std::shared_ptr<ModuleNode>> modules)
{

    this->avail_modules = modules;
}

void ProgramNode::Accept(Visitor *v)
{
    v->Visit(this);
}

std::optional<std::shared_ptr<ModuleNode>> ProgramNode::getMainModule()
{

    for (const auto &[key, current_module] : this->avail_modules)
    {
        for (int i = 0; i < current_module->functions.size(); i++)
        {
            auto c = dynamic_cast<FunctionNode *>(current_module->functions[i].get());
            if (c->f->FunctionName.value == "main")
            {
                return current_module;
            }
        }
    }
    return {};
}

std::optional<std::shared_ptr<FunctionNode>> ProgramNode::getMainFunction()
{
    for (const auto &[key, current_module] : this->avail_modules)
    {
        for (int i = 0; i < current_module->functions.size(); i++)
        {
            auto c = dynamic_cast<FunctionNode *>(current_module->functions[i].get());
            if (c->f->FunctionName.value == "main")
            {
                return std::shared_ptr<FunctionNode>(c);
            }
        }
    }
    return {};
}

std::string ProgramNode::to_string()
{
    return std::string();
}

ConditionalNode::ConditionalNode()
{
}

ConditionalNode::ConditionalNode(std::vector<std::shared_ptr<BranchNode>> branches, std::shared_ptr<Type> type, NodeLocation location) : ASTNode(location)
{
    this->branches = branches;
    this->type = type;
    this->node = location;
}

void ConditionalNode::Accept(Visitor *v)
{
    v->Visit(this);
}

std::string ConditionalNode::to_string()
{
    return std::string();
}

BranchNode::BranchNode()
{
}

BranchNode::BranchNode(std::shared_ptr<ASTNode> condition,
                       std::shared_ptr<BlockNode> stmnts)
{
    this->stmnts = stmnts;
    this->condition = condition;
}

void BranchNode::Accept(Visitor *v)
{
    v->Visit(this);
}

std::string BranchNode::to_string()
{
    return std::string();
}
BlockNode::BlockNode()
{
}
BlockNode::BlockNode(std::vector<std::shared_ptr<FunctionNode>> functions, std::shared_ptr<ReturnNode> exit)

{
    this->functions = functions;
    this->exit = exit;
}
void BlockNode::Accept(Visitor *v)
{
    v->Visit(this);
}
std::string BlockNode::to_string()
{
    return std::string();
}