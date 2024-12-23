#include <Frontend/Ast.h>
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
void IntegerNode::Accept() {}

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
void ExprNode::Accept()
{
}

std::string ExprNode::to_string()
{
    std::map<TokenType, std::string> token_map;

    token_map[TokenType::Addition] = "+";
    token_map[TokenType::Subtraction] = "-";
    token_map[TokenType::Multiplication] = "*";
    token_map[TokenType::Division] = "/";
    return lhs->to_string() + token_map[operation.type] + rhs->to_string();
}

FunctionRefNode::FunctionRefNode(Tokens name,
                                 std::vector<std::shared_ptr<FunctionRefNode>> params,
                                 std::shared_ptr<Type> returnType)
{
}

void FunctionRefNode::Accept()
{
}

std::string FunctionRefNode::to_string()
{
    return std::string();
}

FunctionNode::FunctionNode(std::shared_ptr<FunctionRefNode> functionHeader, std::vector<std::shared_ptr<ASTNode>> stmnts)
{
}

void FunctionNode::Accept()
{
}

std::string FunctionNode::to_string()
{
    return std::string();
}

std::string NativeType::to_string()
{
    return std::string();
}
FunctionType::FunctionType(std::shared_ptr<Type> params,
                           std::shared_ptr<Type> returnType)
{
}
NativeType::NativeType(Tokens type)
{
}
std::string FunctionType::to_string()
{
    return std::string();
}

std::string Type::to_string()
{
    return "";
}
