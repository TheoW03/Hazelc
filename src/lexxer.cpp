#include <vector>
#include <map>
#include <iostream>
enum TokenType
{
    Number,
    Addition,
    Subtraction,
    Multiplication,
    Division,
    Tab
};

struct Tokens
{
    std::string value;
    TokenType type;
};
struct Lexxer_Context
{
    int line_num;
    int state;
    std::string buffer;
    std::vector<Tokens> tokens;
};
void is_number(Lexxer_Context &ctx, char value)
{
    if (value == '-' && ctx.buffer.size() == 0)
    {
        ctx.buffer += value;
    }
    else if (value == '+' || value == '-' || value == '*' || value == '/')
    {
        ctx.tokens.push_back({ctx.buffer, TokenType::Number});
        ctx.buffer = "";
        ctx.buffer += value;
        ctx.state = 2;
    }
    else
    {
        ctx.buffer += value;
    }
}
void is_space(Lexxer_Context &ctx, char value)
{
    if (ctx.buffer.size() == 4)
    {
        ctx.tokens.push_back({"tab", TokenType::Tab});
        ctx.buffer = "";
    }
    if (value != ' ')
    {
        ctx.state = 1;
        ctx.buffer = "";
        ctx.buffer += value;
    }
    else
        ctx.buffer += value;
}
std::vector<Tokens> lexxer(std::vector<std::string> lines)
{
    std::vector<Tokens> tokens;
    Lexxer_Context ctx = {1, 0, "", tokens};
    int state = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].size(); j++)
        {
            auto current_char = lines[i].at(j);
            if (ctx.state == 0)
            {
                is_space(ctx, current_char);
            }
            else if (ctx.state == 1)
            {
                is_number(ctx, current_char);
            }
        }
        ctx.state = 0;
        ctx.buffer = "";
    }
    return ctx.tokens;
}
void print_tokens(std::vector<Tokens> tokens)
{
    std::map<TokenType, std::string> token_map;
    token_map[TokenType::Number] = "Number";
    token_map[TokenType::Tab] = "Tab";

    for (int i = 0; i < tokens.size(); i++)
    {
        std::cout << token_map[tokens[i].type] << ": " << tokens[i].value << std::endl;
    }
}
