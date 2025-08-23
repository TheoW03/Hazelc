#include <vector>
#include <map>
#include <iostream>
#include <unordered_set>
#include <Frontend/Token.h>

// lexer uses a state machine.
// state 0 takes care of indents
// state 1 takes care of numbers
// sttae 2 operators
// state 3 equal operators (=,=\,=>, <=, >=)
// state 4 dot operator
struct Lexxer_Context
{
    int line_num;
    int state;
    std::string buffer;
    std::vector<Tokens> tokens;
    int indents_idx;
    int indents_num;
    std::string file_name;
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
// this is a hashmap of all possible keywords. and symbols
// when it time to add what it does is check if its in the map, then number then ident

std::map<std::string, TokenType> get_keyword_map()
{
    std::map<std::string, TokenType> token_map;

    token_map["+"] = TokenType::Addition;
    token_map["-"] = TokenType::Subtraction;
    token_map["*"] = TokenType::Multiplication;
    token_map["/"] = TokenType::Division;
    token_map["%"] = TokenType::Modulas;
    token_map["++"] = TokenType::Concation;
    token_map["!!"] = TokenType::Index_In;

    token_map[">>"] = TokenType::Right_Shift;
    token_map["<<"] = TokenType::Left_Shift;
    token_map["and"] = TokenType::And;
    token_map["or"] = TokenType::Or;

    token_map["("] = TokenType::Open_Parenthesis;
    token_map[")"] = TokenType::Close_Parenthesis;

    token_map["="] = TokenType::EQ;
    token_map["/="] = TokenType::NE;

    token_map["=>"] = TokenType::Arrow;
    token_map["<="] = TokenType::LTE;
    token_map[">="] = TokenType::GTE;
    token_map[">"] = TokenType::GT;
    token_map["<"] = TokenType::LT;

    token_map["true"] = TokenType::True;
    token_map["false"] = TokenType::False;

    token_map["let"] = TokenType::Let;
    token_map["conditional"] = TokenType::Conditional;
    token_map["integer"] = TokenType::Integer;
    token_map["decimal"] = TokenType::Decimal;
    token_map["byte"] = TokenType::Byte;
    token_map["unsigned_integer"] = TokenType::Uinteger;
    token_map["unsigned_byte"] = TokenType::Ubyte;
    token_map["boolean"] = TokenType::boolean;
    token_map["character"] = TokenType::character;
    token_map["string"] = TokenType::string;

    token_map["true"] = TokenType::True;
    token_map["false"] = TokenType::False;

    token_map["return"] = TokenType::Return;
    token_map["|"] = TokenType::cont_line;
    token_map[":"] = TokenType::Colon;
    token_map[","] = TokenType::Comma;

    token_map["$defualt"] = TokenType::Default;

    token_map["None"] = TokenType::None;
    token_map["["] = TokenType::Open_Bracket;
    token_map["]"] = TokenType::Closed_Bracket;
    token_map["module"] = TokenType::Module;

    token_map["define"] = TokenType::Define;
    token_map[".."] = TokenType::Range;
    token_map["Type"] = TokenType::Generics;
    token_map["export"] = TokenType::Export;
    token_map["instance"] = TokenType::Instance_Function;

    return token_map;
}
bool is_dedent(Lexxer_Context &ctx)
{
    return ctx.indents_idx < ctx.indents_num;
}

bool is_indent(Lexxer_Context &ctx)
{
    return ctx.indents_idx > ctx.indents_num;

    // if (ctx.indents_idx > ctx.indents_num)
    // {
    //     // ctx.indents_num = ctx.indents_idx;
    //     // ctx.indents_idx = 0;
    //     // ctx.tokens.push_back({"Indent", TokenType::Indents, ctx.line_num});
    //     // ctx.buffer = "";
    // }
    // else if (ctx.indents_idx < ctx.indents_num)
    // {
    //     ctx.indents_num = ctx.indents_idx;
    //     ctx.indents_idx = 0;

    //     ctx.tokens.push_back({"dedent", TokenType::Dedents, ctx.line_num});
    //     ctx.buffer = "";
    // }
}
void handle_indents(Lexxer_Context &ctx)
{
    if (is_indent(ctx))
    {
        ctx.indents_num = ctx.indents_idx;
        ctx.tokens.push_back({"Indent", TokenType::Indents, ctx.line_num});
    }
    else if (is_dedent(ctx))
    {
        ctx.indents_num = ctx.indents_idx;
        ctx.tokens.push_back({"Dedent", TokenType::Dedents, ctx.line_num});
    }
    ctx.indents_idx = 0;

    ctx.buffer = "";
}
void is_token(Lexxer_Context &ctx)
{
    if (ctx.buffer == "")
        return;
    std::map<std::string, TokenType> token_map = get_keyword_map();
    if (token_map.find(ctx.buffer) != token_map.end())
        ctx.tokens.push_back({ctx.buffer, token_map[ctx.buffer], ctx.line_num, ctx.file_name});
    else if (is_hex_digit(ctx.buffer))
        ctx.tokens.push_back({ctx.buffer.substr(2, ctx.buffer.size()), TokenType::HexDigit, ctx.line_num, ctx.file_name});
    else if (is_binary_digit(ctx.buffer))
        ctx.tokens.push_back({ctx.buffer.substr(2, ctx.buffer.size()), TokenType::BinaryDigit, ctx.line_num, ctx.file_name});
    else if (is_base_ten(ctx.buffer))
        ctx.tokens.push_back({ctx.buffer, TokenType::BaseTenDigit, ctx.line_num, ctx.file_name});
    // else if (is_indent(ctx) && ctx.state == 0)
    // {
    //     ctx.indents_num = ctx.indents_idx;
    //     ctx.indents_idx = 0;
    //     ctx.tokens.push_back({"Indent", TokenType::Indents, ctx.line_num});
    // }
    // else if (is_dedent(ctx) && ctx.state == 0)
    // {
    //     ctx.indents_num = ctx.indents_idx;
    //     ctx.indents_idx = 0;
    //     ctx.tokens.push_back({"dedent", TokenType::Dedents, ctx.line_num});
    // }
    // else if (ctx.indents_idx == ctx.indents_num && ctx.state == 0)
    // {
    //     ctx.indents_idx = 0;
    // }
    else
        ctx.tokens.push_back({ctx.buffer,
                              TokenType::Identifier,
                              ctx.line_num, ctx.file_name});
    ctx.buffer = "";
}

void is_operand(Lexxer_Context &ctx, char value)
{
    if ((value == '+' && ctx.buffer == "+")     // concat
        || (value == '!' && ctx.buffer == "!")) // Index in
    {
        ctx.buffer += value;
        is_token(ctx);
        ctx.state = 1;
    }
    else if (value == '=')
    {
        ctx.buffer += value;
        ctx.state = 3;
    }
    else
    {

        is_token(ctx);
        ctx.buffer += value;
        ctx.state = 1;

        if (value != '(' && value != ')')
            ctx.state = 1;
        else
            ctx.state = 2;
    }
}
void is_equal(Lexxer_Context &ctx, char value)
{

    if (value == '=' || value == '>' || value == '<' || value == '/')
    {

        ctx.buffer += value;
    }
    else
    {
        // void is_number(Lexxer_Context & ctx, char value);

        is_token(ctx);
        ctx.buffer += value;
        if (value == '(')
        {
            is_token(ctx);
        }
        std::cout << "buffer: " << ctx.buffer << std::endl;
        // is_number(ctx, value);
        ctx.state = 1;
    }
}

void is_number(Lexxer_Context &ctx, char value)
{

    if (value == '-' && ctx.buffer.size() == 0)
    {
        ctx.buffer += value;
    }
    else if (value == '.')
    {
        // ctx.buffer += value;
        ctx.state = 4;
    }
    else if (value == '/')
    {
        is_token(ctx);
        ctx.state = 3;
        ctx.buffer += value;
    }
    else if (value == '+' || value == '*' || value == '-' || value == '/' || value == '%' || value == '(' || value == ')' || value == '!')
    {

        is_token(ctx);
        ctx.buffer += value;
        ctx.state = 2;
    }
    else if (value == ':' || value == ',' || value == '[' || value == ']')
    {
        is_token(ctx);
        ctx.buffer += value;
        is_token(ctx);
    }
    else if (value == '=' || value == '>' || value == '<')
    {

        is_token(ctx);
        ctx.buffer += value;
        ctx.state = 3;
    }
    else
    {
        ctx.buffer += value;
    }
}

void is_space(Lexxer_Context &ctx, char value)
{
    // DEBUG:
    // std::cout << "num: " << ctx.indents_num << std::endl;
    // std::cout << "indnets idx: " << ctx.indents_idx << std::endl;
    // std::cout << "line num: " << ctx.line_num << std::endl;
    if (ctx.buffer.size() == 4)
    {

        ctx.indents_idx++;

        ctx.buffer = "";
    }

    if (value != ' ')
    {
        if (ctx.indents_idx > ctx.indents_num)
        {
            ctx.indents_num = ctx.indents_idx;
            ctx.indents_idx = 0;
            ctx.tokens.push_back({"Indent", TokenType::Indents, ctx.line_num});
            ctx.buffer = "";
        }
        else if (ctx.indents_idx < ctx.indents_num)
        {
            ctx.indents_num = ctx.indents_idx;
            ctx.indents_idx = 0;

            ctx.tokens.push_back({"dedent", TokenType::Dedents, ctx.line_num});
            ctx.buffer = "";
        }
        ctx.indents_idx = 0;
        ctx.state = 1;
        ctx.buffer = "";

        // ctx.indents_num = 0;
        is_number(ctx, value);
    }
    else
        ctx.buffer += value;
}

void dot_state(Lexxer_Context &ctx, char value)
{
    if (value == '.')
    {
        is_token(ctx);
        ctx.buffer += "..";
        is_token(ctx);
    }
    else
    {
        ctx.buffer += '.';
        ctx.buffer += value;
    }

    ctx.state = 1;
}
std::vector<Tokens> lexxer(std::vector<std::string> lines, std::string file_name)
{
    std::vector<Tokens> tokens;
    Lexxer_Context ctx = {1, 0, "", tokens};
    ctx.file_name = file_name;
    int state = 0;
    int is_comment = 0;
    int is_str = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].size(); j++)
        {
            auto current_char = lines[i].at(j);
            if (current_char == '(' && lines[i].at(j + 1) == '*')
            {
                is_comment = 1;
            }

            if (current_char == ')' && lines[i].at(j - 1) == '*')
            {
                is_comment = 0;
                continue;
            }
            if (is_comment == 1)
            {
                continue;
            }
            if (current_char == '\"' || current_char == '\'')
            {
                if (is_str == 1 && current_char == '\"')
                {
                    // std::cout << "str: " << ctx.buffer << " state: " << ctx.state << " indents: "
                    //   << ctx.indents_idx
                    //   << " indents num: "
                    //   << ctx.indents_num
                    //   << std::endl;

                    ctx.tokens.push_back({ctx.buffer, TokenType::String_Lit, ctx.line_num});
                    ctx.buffer = "";
                    is_str = 0;
                }
                else if (is_str == 1 && current_char == '\'')
                {

                    ctx.tokens.push_back({ctx.buffer, TokenType::Char_Lit, ctx.line_num});
                    ctx.buffer = "";
                    is_str = 0;
                }
                else
                {
                    if (ctx.state == 0)
                    {
                        // if (is_indent(ctx))
                        // {
                        //     ctx.indents_num = ctx.indents_idx;
                        //     ctx.tokens.push_back({"Indent", TokenType::Indents, ctx.line_num});
                        // }
                        // else if (is_dedent(ctx))
                        // {
                        //     ctx.indents_num = ctx.indents_idx;
                        //     ctx.tokens.push_back({"Dedent", TokenType::Dedents, ctx.line_num});
                        // }
                        // ctx.indents_idx = 0;

                        // ctx.buffer = "";
                        handle_indents(ctx);
                    }
                    else
                    {
                        is_token(ctx);
                    }

                    is_str = 1;
                }
                ctx.state = 1;
                continue;
            }

            if (is_str == 1)
            {
                ctx.buffer += current_char;

                continue;
            }
            if (current_char == ' ' && ctx.state != 0)
            {
                is_token(ctx);
                continue;
            }
            if (ctx.state == 0)
            {
                is_space(ctx, current_char);
            }
            else if (ctx.state == 1)
            {
                // std::cout << "state 1 " << " current char: " << current_char << std::endl;

                is_number(ctx, current_char);
            }
            else if (ctx.state == 2)
            {
                // std::cout << "state 2 " << "current char: " << current_char << std::endl;

                is_operand(ctx, current_char);
            }
            else if (ctx.state == 3)
            {
                // std::cout << "state 3 " << "current char: " << current_char << std::endl;

                is_equal(ctx, current_char);
            }
            else if (ctx.state == 4)
            {
                // std::cout << "state 4 " << "current char: " << current_char << std::endl;

                dot_state(ctx, current_char);
            }
        }
        // is_space(ct)
        ctx.line_num++;
        ctx.state = 0;
        is_token(ctx);
        ctx.buffer = "";
    }
    handle_indents(ctx);
    // std::cout << ctx.indents_num << std::endl;
    // if (ctx.indents_idx > ctx.indents_num)
    // {
    //     ctx.indents_num = ctx.indents_idx;
    //     ctx.indents_idx = 0;
    //     ctx.tokens.push_back({"Indent", TokenType::Indents, ctx.line_num});
    //     ctx.buffer = "";
    // }
    // else if (ctx.indents_idx < ctx.indents_num)
    // {
    //     ctx.indents_num = ctx.indents_idx;
    //     ctx.indents_idx = 0;

    //     ctx.tokens.push_back({"dedent", TokenType::Dedents, ctx.line_num});
    //     ctx.buffer = "";
    // }
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

    token_map[TokenType::Identifier] = "Identifier";
    token_map[TokenType::Open_Parenthesis] = "Open_paren";
    token_map[TokenType::Close_Parenthesis] = "Close_paren";
    token_map[TokenType::Integer] = "integer";
    token_map[TokenType::Decimal] = "decimal";
    token_map[TokenType::Conditional] = "conditional";
    token_map[TokenType::Let] = "let";
    token_map[TokenType::Indents] = "indent";
    token_map[TokenType::Dedents] = "dedent";
    token_map[TokenType::Colon] = "Colon";
    token_map[TokenType::Return] = "return";
    token_map[TokenType::Default] = "defualt";
    token_map[TokenType::Comma] = "comma";
    token_map[TokenType::Arrow] = "Arrow";
    token_map[TokenType::GTE] = "Gte";
    token_map[TokenType::GT] = "Gt";
    token_map[TokenType::LT] = "Lt";
    token_map[TokenType::LTE] = "LTE";
    token_map[TokenType::EQ] = "EQ";
    token_map[TokenType::NE] = "NE";
    token_map[TokenType::Module] = "module";

    token_map[TokenType::Right_Shift] = "right_shift";
    token_map[TokenType::Left_Shift] = "left_shift";
    token_map[TokenType::And] = "and";
    token_map[TokenType::Or] = "or";
    token_map[TokenType::True] = "true";

    token_map[TokenType::cont_line] = "contine";
    token_map[TokenType::Open_Bracket] = "Open bracket";
    token_map[TokenType::Closed_Bracket] = "Closed bracket";
    token_map[TokenType::Range] = "Range";
    token_map[TokenType::String_Lit] = "str_lit";
    token_map[TokenType::Char_Lit] = "char_lit";
    token_map[TokenType::Export] = "export";

    for (int i = 0; i < tokens.size(); i++)
    {
        std::cout << token_map[tokens[i].type] << ": \"" << tokens[i].value << "\": " << tokens[i].line_num << ": " << tokens[i].file_name << std::endl;
    }
}
