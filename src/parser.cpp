#include <vector>
#include <optional>
#include <Frontend/lexxer.h>
#include <iostream>
#include <Frontend/Ast.h>
#include <map>
#include <stdint.h>

std::optional<Tokens> current;

std::optional<std::shared_ptr<ASTNode>> expression(std::vector<Tokens> &tokens);
std::optional<std::shared_ptr<FunctionRefNode>> parse_function_ref(std::vector<Tokens> &tokens);
using parser = std::optional<std::shared_ptr<ASTNode>> (*)(std::vector<Tokens> &);

std::optional<Tokens> match_and_remove(TokenType token_type, std::vector<Tokens> &tokens)
{
    if (tokens.empty())
        return {};
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
Tokens get_next_token(std::vector<Tokens> &tokens)
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
    else if (match_and_remove(TokenType::None, tokens).has_value())
    {
        return std::make_shared<NoneNode>();
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
    auto lhs = factor(tokens);
    auto op = match_and_remove({TokenType::Multiplication, TokenType::Division}, tokens);
    while (op.has_value())
    {
        auto rhs = factor(tokens);
        lhs = std::make_shared<ExprNode>(lhs.value(), op.value(), rhs.value());
        op = match_and_remove({TokenType::Multiplication, TokenType::Division}, tokens);
    }
    return lhs;
}
std::optional<std::shared_ptr<ASTNode>> expression(std::vector<Tokens> &tokens)
{
    auto lhs = term(tokens);
    auto op = match_and_remove({TokenType::Addition, TokenType::Subtraction}, tokens);
    while (op.has_value())
    {
        auto rhs = term(tokens);
        lhs = std::make_shared<ExprNode>(lhs.value(), op.value(), rhs.value());
        op = match_and_remove({TokenType::Addition, TokenType::Subtraction}, tokens);
    }
    return lhs;
}

std::vector<std::shared_ptr<FunctionRefNode>> parse_params(std::vector<Tokens> &tokens)
{
    std::vector<std::shared_ptr<FunctionRefNode>> params;

    if (!match_and_remove(TokenType::Open_Parenthesis, tokens)
             .has_value())
    {
        return params;
    }
    while (!match_and_remove(TokenType::Close_Parenthesis, tokens).has_value())
    {
        match_and_remove(TokenType::Let, tokens);
        params.push_back(parse_function_ref(tokens)
                             .value());
        match_and_remove(TokenType::Comma, tokens);
        // print_tokens(tokens);
    }

    return params;
}
std::optional<std::shared_ptr<Type>> parse_type(std::vector<Tokens> &tokens)
{
    std::vector<TokenType> typeVector = {TokenType::Integer,
                                         TokenType::Decimal,
                                         TokenType::Ubyte,
                                         TokenType::Uinteger,
                                         TokenType::Byte,
                                         TokenType::boolean,
                                         TokenType::character,
                                         TokenType::string};
    if (look_ahead(typeVector, tokens))
    {

        auto ty = match_and_remove(typeVector, tokens);
        return std::make_shared<NativeType>(ty.value());
    }
    else if (look_ahead(TokenType::Open_Parenthesis, tokens))
    {

        // print_tokens(tokens);
        std::vector<std::shared_ptr<Type>> type_params;
        while (!match_and_remove(TokenType::Close_Parenthesis, tokens).has_value())
        {
            type_params.push_back(parse_type(tokens)
                                      .value());
            match_and_remove(TokenType::Comma, tokens);
        }
        match_and_remove(TokenType::Colon, tokens);
        auto retty = parse_type(tokens).value();
        return std::make_shared<FunctionType>(type_params, retty);
    }
    else if (look_ahead(TokenType::Open_Bracket, tokens))
    {
        // print_tokens(tokens);
        return std::make_shared<ListType>(parse_type(tokens).value());
    }
    return {};
}
std::optional<std::shared_ptr<FunctionRefNode>> parse_function_ref(std::vector<Tokens> &tokens)
{
    auto name = match_and_remove(TokenType::Identifier, tokens);
    auto params = parse_params(tokens);
    match_and_remove(TokenType::Colon, tokens);

    auto ret_type = parse_type(tokens);
    return std::make_shared<FunctionRefNode>(name.value(), params, ret_type.value());
}
std::vector<std::shared_ptr<ASTNode>> parse_scope(std::vector<Tokens> &tokens)
{
    std::vector<std::shared_ptr<ASTNode>> ast;
    if (match_and_remove(TokenType::Indents, tokens))
    {
        while (!match_and_remove(TokenType::Dedents, tokens).has_value() && get_next_token(tokens).type != TokenType::EndOfFile)
        {
            /* code */
            std::optional<std::shared_ptr<ASTNode>>
                parse_stmnts(std::vector<Tokens> & tokens);

            auto v = parse_stmnts(tokens);
            ast.push_back(v.value());
        }
    }
    else if (match_and_remove(TokenType::Arrow, tokens).has_value())
    {
        ast.push_back(std::make_shared<ReturnNode>(expression(tokens).value()));
    }
    else
    {
        std::cout << "missing dedent or arrow on line or indent " << tokens[0].line_num << std::endl;
        exit(EXIT_FAILURE);
    }
    return ast;
}
std::optional<std::shared_ptr<ASTNode>> parse_function(std::vector<Tokens> &tokens)
{
    match_and_remove(TokenType::Let, tokens);
    auto func = parse_function_ref(tokens);
    std::vector<std::shared_ptr<ASTNode>> ast;

    return std::make_shared<FunctionNode>(func.value(),
                                          parse_scope(tokens));
}
std::optional<std::shared_ptr<ASTNode>> parse_return(std::vector<Tokens> &tokens)
{
    match_and_remove(TokenType::Return, tokens);
    return std::make_shared<ReturnNode>(expression(tokens).value());
}

std::optional<std::shared_ptr<ASTNode>> parse_stmnts(std::vector<Tokens> &tokens)
{
    using std::make_pair; // here bc im lazy you may use using on the stack level. but lets stick to this

    std::map<TokenType, parser> parse_map;
    parse_map.insert(make_pair(TokenType::Let, (parser)parse_function)); //
    parse_map.insert(make_pair(TokenType::Return, (parser)parse_return));
    if (parse_map.count(get_next_token(tokens).type))
    {
        return parse_map.at(get_next_token(tokens).type)(tokens); // meow :3
    }
    else
    {
        std::cout << "unexpected identifier \"" << get_next_token(tokens).value << "\" on line " << tokens[0].line_num << std::endl;
        exit(EXIT_FAILURE);
    }
}
std::shared_ptr<ModuleNode> parse_module(std::vector<Tokens> &tokens)
{
    auto module_name = match_and_remove(TokenType::Identifier, tokens);
    // print_tokens(tokens);

    std::vector<std::shared_ptr<ASTNode>> functions;
    while (!look_ahead(TokenType::EndOfFile, tokens) && !look_ahead(TokenType::Module, tokens))
    {
        if (match_and_remove(TokenType::Let, tokens).has_value())
        {
            functions.push_back(parse_function(tokens).value());
        }
    }
    return std::make_shared<ModuleNode>(functions, module_name.value());
}
std::vector<std::shared_ptr<ModuleNode>> parse_node(std::vector<Tokens> &tokens)
{
    std::vector<std::shared_ptr<ModuleNode>> modules;
    while (!match_and_remove(TokenType::EndOfFile, tokens).has_value() && tokens.size() != 0)
    {
        if (match_and_remove(TokenType::Module, tokens).has_value())
        {
            auto m = parse_module(tokens);

            modules.push_back(m);
        }
    }
    return modules;

    // return modules(tokens);
}
