#include <iostream>

#ifndef TOKEN_TYPE
#define TOKEN_TYPE
enum TokenType
{
    Addition,
    Subtraction,
    Multiplication,
    Division,
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
    NE,
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
    string,
    Modulas,
    And,
    Or,
    True,
    False,
    Left_Shift,
    Right_Shift,
    Define,
    Range,
    Lambda,
    Generics,
    String_Lit,
    Char_Lit

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