#include <vector>
#include <optional>
#include <Frontend/lexxer.h>
#include <iostream>
#include <Frontend/Ast.h>
#include <map>
#include <stdint.h>

std::optional<Tokens> current;

std::optional<std::shared_ptr<ASTNode>> expr_parse(std::vector<Tokens> &tokens);
std::optional<std::shared_ptr<FunctionRefNode>> parse_function_ref(std::vector<Tokens> &tokens);
using parser = std::optional<std::shared_ptr<ASTNode>> (*)(std::vector<Tokens> &);
std::optional<std::shared_ptr<ASTNode>> parse_bitshift(std::vector<Tokens> &tokens);

// This is a typical recursive decent parser
// how it works is program node is the root node

// match and romve has been overloaded to take a vector
// what it does in that codase is iterate the vector if it contians one tokenType of the 0th element of the list of tokens
// it removes it and returns it

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
std::optional<std::shared_ptr<ASTNode>> parse_list(std::vector<Tokens> &tokens)
{
    std::vector<std::shared_ptr<ASTNode>> values;
    while (!match_and_remove(TokenType::Closed_Bracket, tokens).has_value())
    {
        values.push_back(expr_parse(tokens)
                             .value());
        match_and_remove(TokenType::Comma, tokens);
    }
    return std::make_shared<ListNode>(values);
}
std::vector<std::shared_ptr<ASTNode>> parse_call_params(std::vector<Tokens> &tokens)
{
    std::vector<std::shared_ptr<ASTNode>> params;
    if (match_and_remove(TokenType::Open_Parenthesis, tokens).has_value())
    {
        while (!match_and_remove(TokenType::Close_Parenthesis, tokens).has_value())
        {
            params.push_back(expr_parse(tokens).value());
            match_and_remove(TokenType::Comma, tokens);
            /* code */
        }
    }
    return params;
}
std::optional<std::shared_ptr<ASTNode>> parse_function_call(std::vector<Tokens> &tokens)
{
    auto name = match_and_remove(TokenType::Identifier, tokens);
    return std::make_shared<FunctionCallNode>(name.value(), parse_call_params(tokens));
}

std::optional<std::shared_ptr<ASTNode>> factor(std::vector<Tokens> &tokens)
{

    if (look_ahead({TokenType::BaseTenDigit, TokenType::HexDigit, TokenType::BinaryDigit}, tokens))
    {
        auto number = match_and_remove({TokenType::BaseTenDigit,
                                        TokenType::HexDigit,
                                        TokenType::BinaryDigit},
                                       tokens);
        if (number.value().value.find('.') != std::string::npos)
        {
            return std::make_shared<DecimalNode>(number.value());
        }
        return std::make_shared<IntegerNode>(number.value());
    }
    else if (match_and_remove(TokenType::None, tokens).has_value())
    {
        return std::make_shared<NoneNode>();
    }
    else if (match_and_remove(TokenType::Open_Bracket, tokens).has_value())
    {
        return parse_list(tokens);
    }
    else if (match_and_remove({TokenType::True, TokenType::False}, tokens).has_value())
    {
        return std::make_shared<BooleanConstNode>(current.value());
    }
    else if (match_and_remove(TokenType::String_Lit, tokens).has_value())
    {
        return std::make_shared<StringNode>(current.value());
    }
    else if (match_and_remove(TokenType::Char_Lit, tokens).has_value())
    {
        return std::make_shared<CharNode>(current.value());
    }
    else if (look_ahead(TokenType::Identifier, tokens))
    {
        return parse_function_call(tokens);
    }
    else if (look_ahead(TokenType::Open_Parenthesis, tokens))
    {
        match_and_remove(TokenType::Open_Parenthesis, tokens);
        auto ret = expr_parse(tokens);
        match_and_remove(TokenType::Close_Parenthesis, tokens);
        return ret;
    }
    return {};
}
std::optional<std::shared_ptr<ASTNode>> BoolExpr(std::vector<Tokens> &tokens)
{
    auto bool_expr_tokens = {
        TokenType::LT,
        TokenType::GT,
        TokenType::LTE,
        TokenType::GTE,
        TokenType::EQ,
        TokenType::NE};
    auto lhs = factor(tokens);
    auto op = match_and_remove(bool_expr_tokens,
                               tokens);
    if (op.has_value())
    {
        auto rhs = factor(tokens);
        lhs = std::make_shared<BooleanExprNode>(lhs.value(), op.value(), rhs.value());
        // op = match_and_remove(bool_expr_tokens,
        //   tokens);
    }
    return lhs;
}

std::optional<std::shared_ptr<ASTNode>> term(std::vector<Tokens> &tokens)
{
    auto term_tokens = {
        TokenType::Multiplication,
        TokenType::Division,
        TokenType::Modulas,

    };

    // TokenType::And,
    // TokenType::Or,
    // TokenType::Left_Shift,
    // TokenType::Right_Shift};
    auto lhs = BoolExpr(tokens);
    auto op = match_and_remove(term_tokens,
                               tokens);
    while (op.has_value())
    {
        auto rhs = BoolExpr(tokens);
        lhs = std::make_shared<ExprNode>(lhs.value(), op.value(), rhs.value());
        op = match_and_remove(term_tokens,
                              tokens);
    }
    return lhs;
}

std::optional<std::shared_ptr<ASTNode>> expression(std::vector<Tokens> &tokens)
{
    auto lhs = term(tokens);
    auto expression_tokens = {
        TokenType::Addition,
        TokenType::Subtraction,
        TokenType::Concation,
        TokenType::Left_Shift,
        TokenType::Right_Shift,
        TokenType::And,
        TokenType::Or

    };
    auto op = match_and_remove(expression_tokens, tokens);
    while (op.has_value())
    {
        auto rhs = term(tokens);
        lhs = std::make_shared<ExprNode>(lhs.value(), op.value(), rhs.value());
        op = match_and_remove(expression_tokens, tokens);
    }
    return lhs;
}
// std::optional<std::shared_ptr<ASTNode>> parse_bitshift(std::vector<Tokens> &tokens)
// {
//     auto lhs = expression(tokens);
//     auto expression_tokens = {
//         TokenType::Left_Shift,
//         TokenType::Right_Shift};
//     auto op = match_and_remove(expression_tokens, tokens);
//     while (op.has_value())
//     {
//         auto rhs = expression(tokens);
//         lhs = std::make_shared<ExprNode>(lhs.value(), op.value(), rhs.value());
//         op = match_and_remove(expression_tokens, tokens);
//     }
//     return lhs;
// }
std::optional<std::shared_ptr<BranchNode>> parse_branch(std::vector<Tokens> &tokens)
{
    std::vector<std::shared_ptr<ASTNode>> parse_scope(std::vector<Tokens> & tokens);
    auto condition = expr_parse(tokens);
    auto stmnts = parse_scope(tokens);
    return std::make_shared<BranchNode>(condition.value(), stmnts);
}
std::optional<std::shared_ptr<ConditionalNode>> parse_conditional(std::vector<Tokens> &tokens)
{
    std::optional<std::shared_ptr<Type>> parse_type(std::vector<Tokens> & tokens);
    match_and_remove(TokenType::Colon, tokens);
    auto type = parse_type(tokens);
    std::vector<std::shared_ptr<BranchNode>> c;
    match_and_remove(TokenType::Indents, tokens);
    while (!look_ahead(TokenType::Default, tokens) &&
           (!match_and_remove(TokenType::Dedents, tokens).has_value() //
            && !look_ahead(TokenType::EndOfFile, tokens)))
    {

        c.push_back(parse_branch(tokens).value());
    }
    if (match_and_remove(TokenType::Default, tokens).has_value())
    {

        std::vector<std::shared_ptr<ASTNode>> parse_scope(std::vector<Tokens> & tokens);

        Tokens fake_bool = {"true", TokenType::True, get_next_token(tokens).line_num};

        auto s = parse_scope(tokens);
        auto condutio = std::make_shared<BooleanConstNode>(fake_bool);
        c.push_back(std::make_shared<BranchNode>(condutio, s));
    }
    else
    {
        std::cout << "hazelc: conditionals must always have a defualt condition, denoted as $defualt" << std::endl;
        std::cout << "hazelc: compilation terminated" << std::endl;
        exit(EXIT_FAILURE);
    }
    match_and_remove(TokenType::Dedents, tokens);
    return std::make_shared<ConditionalNode>(c, type.value());
}
std::optional<std::shared_ptr<ASTNode>> expr_parse(std::vector<Tokens> &tokens)
{
    if (match_and_remove(TokenType::Conditional, tokens).has_value())
    {
        return parse_conditional(tokens);
    }
    else
    {
        return expression(tokens);
    }
    return {};
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
    else if (match_and_remove(TokenType::Open_Bracket, tokens).has_value())
    {
        // print_tokens(tokens);
        auto d = std::make_shared<ListType>(parse_type(tokens).value());
        match_and_remove(TokenType::Closed_Bracket, tokens);
        return d;
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
            // print_tokens(tokens);

            std::optional<std::shared_ptr<ASTNode>>
                parse_stmnts(std::vector<Tokens> & tokens);
            auto v = parse_stmnts(tokens);

            ast.push_back(v.value());
        }
    }
    else if (match_and_remove(TokenType::Arrow, tokens).has_value())
    {

        ast.push_back(std::make_shared<ReturnNode>(expr_parse(tokens).value()));
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
    bool is_export = current.value().type == TokenType::Export;

    match_and_remove(TokenType::Let, tokens);
    auto func = parse_function_ref(tokens);
    std::vector<std::shared_ptr<ASTNode>> ast;

    return std::make_shared<FunctionNode>(is_export, func.value(),
                                          parse_scope(tokens));
}
std::optional<std::shared_ptr<ASTNode>> parse_return(std::vector<Tokens> &tokens)
{
    match_and_remove(TokenType::Return, tokens);
    return std::make_shared<ReturnNode>(expr_parse(tokens).value());
}

std::optional<std::shared_ptr<ASTNode>> parse_stmnts(std::vector<Tokens> &tokens)
{
    using std::make_pair; // here bc im lazy you may use using on the stack level. but lets stick to this

    std::map<TokenType, parser> parse_map;
    parse_map.insert(make_pair(TokenType::Let, (parser)parse_function)); //
    parse_map.insert(make_pair(TokenType::Return, (parser)parse_return));
    parse_map.insert(make_pair(TokenType::Identifier, (parser)parse_function_call));

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

    // while()
    std::vector<Tokens> imports;
    if (match_and_remove(TokenType::Open_Parenthesis, tokens).has_value())
    {
        while (!match_and_remove(TokenType::Close_Parenthesis, tokens).has_value())
        {
            auto c = match_and_remove(TokenType::Identifier, tokens);
            imports.push_back(c.value());
            match_and_remove(TokenType::Comma, tokens);
        }
    }

    std::vector<std::shared_ptr<ASTNode>> functions;

    while (!look_ahead(TokenType::EndOfFile, tokens) && !look_ahead(TokenType::Module, tokens))
    {
        match_and_remove(TokenType::Export, tokens);

        if (look_ahead(TokenType::Let, tokens))
        {
            functions.push_back(parse_function(tokens).value());
        }
    }
    return std::make_shared<ModuleNode>(functions, module_name.value(), imports);
}
std::shared_ptr<ProgramNode> parse_node(std::vector<Tokens> &tokens)
{
    std::map<std::string, std::shared_ptr<ModuleNode>> modules;
    while (!match_and_remove(TokenType::EndOfFile, tokens).has_value() && tokens.size() != 0)
    {
        if (match_and_remove(TokenType::Module, tokens).has_value())
        {
            auto m = parse_module(tokens);
            modules.insert(std::make_pair(m->name.value, m));
        }
    }
    return std::make_shared<ProgramNode>(modules);
}
