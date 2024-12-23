#include <vector>
#include <iostream>

#ifndef TOKEN_TYPE
#define TOKEN_TYPE
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
    Open_Parenthesis,
    Close_Parenthesis,
    Let,
    Integer,
    Decimal,
    Conditional,
    Indents,
    Dedents,
    Return,
    cont_line,
    Colon,
    Default,
    Comma,
    Arrow,
    GT,
    LT,
    LTE,
    EQ,
    GTE

};
#endif

#ifndef TOKENS
#define TOKENS
struct Tokens
{
    std::string value;
    TokenType type;
    int line_num;
};
#endif

std::vector<Tokens> lexxer(std::vector<std::string> lines);
void print_tokens(std::vector<Tokens> tokens);