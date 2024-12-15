#include <vector>
#include <map>
#include <iostream>
#include <unordered_set>

enum TokenType
{
    Addition,
    Subtraction,
    Multiplication,
    Division,
    Tab,
    Identifier,
    HexDigit,
    BinaryDigit,
    BaseTenDigit,
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
bool is_hex_digit(std::string &str)
{
    std::string a = str.substr(0, 2);

    if (a != "0x" && a != "0X")
        return false;

    std::unordered_set<char> hex_digits; // Creates a set of integers
    hex_digits.insert('0');
    hex_digits.insert('1');
    hex_digits.insert('2');
    hex_digits.insert('3');
    hex_digits.insert('4');
    hex_digits.insert('5');
    hex_digits.insert('6');
    hex_digits.insert('7');
    hex_digits.insert('8');
    hex_digits.insert('9');
    hex_digits.insert('a');
    hex_digits.insert('b');
    hex_digits.insert('c');
    hex_digits.insert('d');
    hex_digits.insert('e');
    hex_digits.insert('f');
    hex_digits.insert('A');
    hex_digits.insert('B');
    hex_digits.insert('C');
    hex_digits.insert('D');
    hex_digits.insert('E');
    hex_digits.insert('F');
    std::string c = str.substr(2, str.size() - 1);

    for (char ch : c)
    {
        if (hex_digits.find(ch) == hex_digits.end())
        {
            std::cout << "error with your hex number " << str << ch << std::endl;
            return false;
        }
    }
    return true;
}
bool is_binary_digit(std::string &str)
{
    std::string a = str.substr(0, 2);
    if (a != "0b")
        return false;

    std::unordered_set<char> binary_dig; // Creates a set of integers

    binary_dig.insert('0');
    binary_dig.insert('1');

    for (char ch : str.substr(2, str.size()))
    {
        if (binary_dig.find(ch) == binary_dig.end())
        {
            std::cout << "error with your binary number " << str << std::endl;
            return false;
        }
    }
    return true;
}
bool is_base_ten(std::string &str)
{

    std::unordered_set<char> hex_digits; // Creates a set of integers
    hex_digits.insert('.');
    hex_digits.insert('-');

    hex_digits.insert('0');
    hex_digits.insert('1');
    hex_digits.insert('2');
    hex_digits.insert('3');
    hex_digits.insert('4');
    hex_digits.insert('5');
    hex_digits.insert('6');
    hex_digits.insert('7');
    hex_digits.insert('8');
    hex_digits.insert('9');

    for (char ch : str)
    {
        if (hex_digits.find(ch) == hex_digits.end())
        {
            return false;
        }
    }
    return true;
}
void is_token(Lexxer_Context &ctx)
{
    if (ctx.buffer == "")
        return;
    std::map<std::string, TokenType> token_map;
    token_map["+"] = TokenType::Addition;
    token_map["-"] = TokenType::Subtraction;
    token_map["*"] = TokenType::Multiplication;
    token_map["/"] = TokenType::Division;
    if (is_hex_digit(ctx.buffer))
        ctx.tokens.push_back({ctx.buffer, TokenType::HexDigit});
    else if (is_binary_digit(ctx.buffer))
        ctx.tokens.push_back({ctx.buffer, TokenType::BinaryDigit});
    else if (is_base_ten(ctx.buffer))
        ctx.tokens.push_back({ctx.buffer, TokenType::BaseTenDigit});
    else if (token_map.find(ctx.buffer) != token_map.end())
        ctx.tokens.push_back({ctx.buffer, token_map[ctx.buffer]});
    else
        ctx.tokens.push_back({ctx.buffer, TokenType::Identifier});
    ctx.buffer = "";
}
void is_operand(Lexxer_Context &ctx, char value)
{
    is_token(ctx);
    ctx.buffer += value;
    ctx.state = 1;
}

void is_number(Lexxer_Context &ctx, char value)
{

    if (value == '-' && ctx.buffer.size() == 0)
    {
        ctx.buffer += value;
    }
    else if (value == '+' || value == '-' || value == '*' || value == '/')
    {

        is_token(ctx);
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
            if (current_char == ' ' && ctx.state != 0)
            {
                is_token(ctx);

                ctx.buffer = "";
                continue;
            }
            if (ctx.state == 0)
            {
                std::cout << "11" << std::endl;
                is_space(ctx, current_char);
            }
            else if (ctx.state == 1)
            {
                is_number(ctx, current_char);
            }
            else if (ctx.state == 2)
            {
                is_operand(ctx, current_char);
            }
        }
        ctx.state = 0;
        is_token(ctx);
        ctx.buffer = "";
    }
    return ctx.tokens;
}
void print_tokens(std::vector<Tokens> tokens)
{
    std::map<TokenType, std::string> token_map;
    token_map[TokenType::BinaryDigit] = "BinaryDigit";
    token_map[TokenType::BaseTenDigit] = "BaseTenDigit";
    token_map[TokenType::HexDigit] = "HexDigit";
    token_map[TokenType::Addition] = "Addition";
    token_map[TokenType::Subtraction] = "Subtraction";
    token_map[TokenType::Multiplication] = "Multiplication";
    token_map[TokenType::Division] = "Division";

    token_map[TokenType::Tab] = "Tab";
    token_map[TokenType::Identifier] = "Identifier";

    for (int i = 0; i < tokens.size(); i++)
    {
        std::cout << token_map[tokens[i].type] << ": " << tokens[i].value << std::endl;
    }
}
