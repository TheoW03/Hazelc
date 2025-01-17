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
    GTE,
    None,
    Open_Bracket,
    Closed_Bracket,
    EndOfFile,
    Module,
    Byte,
    Uinteger,
    Ubyte,
    boolean,
    character,
    string

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