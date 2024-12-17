#include <vector>
#include <optional>
#include <Frontend/lexxer.h>
#include <iostream>
#include <Frontend/Ast.h>
std::optional<Tokens> current;
std::optional<std::shared_ptr<ASTNode>> expression(std::vector<Tokens> &tokens);

std::optional<Tokens> match_and_remove(TokenType token_type, std::vector<Tokens> &tokens)
{
    if (tokens[0].type == token_type)
    {
        current = tokens[0];
        tokens.erase(tokens.begin());
        return current;
    }
    return {};
}
std::optional<Tokens> match_and_remove(std::vector<TokenType> token, std::vector<Tokens> &tokens)
{
    for (int i = 0; i < token.size(); i++)
    {
        if (tokens[0].type == token[i])
        {
            current = tokens[0];
            tokens.erase(tokens.begin());
            return current;
        }
    }
    return {};
}

bool look_ahead(TokenType t, std::vector<Tokens> &tokens)
{
    return t == tokens[0].type;
}

bool look_ahead(std::vector<TokenType> token, std::vector<Tokens> &tokens)
{
    for (int i = 0; i < token.size(); i++)
    {
        if (token[i] == tokens[0].type)
        {
            return true;
        }
    }
    return false;
}
std::optional<std::shared_ptr<ASTNode>> factor(std::vector<Tokens> &tokens)
{

    if (look_ahead({TokenType::BaseTenDigit, TokenType::HexDigit, TokenType::BinaryDigit}, tokens))
    {
        auto number = match_and_remove({TokenType::BaseTenDigit,
                                        TokenType::HexDigit,
                                        TokenType::BinaryDigit},
                                       tokens);
        return std::make_shared<IntegerNode>(number.value());
    }
    else if (look_ahead(TokenType::Open_Parenthesis, tokens))
    {
        match_and_remove(TokenType::Open_Parenthesis, tokens);
        auto ret = expression(tokens);
        match_and_remove(TokenType::Close_Parenthesis, tokens);
        return ret;
    }
    return {};
}
std::optional<std::shared_ptr<ASTNode>> term(std::vector<Tokens> &tokens)
{
    return {};
}
std::optional<std::shared_ptr<ASTNode>> expression(std::vector<Tokens> &tokens)
{
    return {};
}

void parse_node(std::vector<Tokens> tokens)
{
    std::cout << "parsing" << std::endl;
}
