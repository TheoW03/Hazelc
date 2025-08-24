#include <vector>
#include <optional>
#include <Frontend/lexxer.h>
#include <iostream>

std::optional<Tokens> match_and_remove(TokenType token_type, std::vector<Tokens> &tokens)
{
    if (tokens.empty())
        return {};
    if (tokens[0].type == token_type)
    {

        auto current = tokens[0];
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
            auto current = tokens[0];
            tokens.erase(tokens.begin());
            return current;
        }
    }
    return {};
}
Tokens peek(std::vector<Tokens> &tokens)
{
    return tokens[0];
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