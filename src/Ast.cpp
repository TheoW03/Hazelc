#include <Frontend/Ast.h>
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
ExprNode::ExprNode(std::shared_ptr<ASTNode> lhs, Tokens operation, std::shared_ptr<ASTNode> rhs)
{
}
void ExprNode::Accept()
{
}
