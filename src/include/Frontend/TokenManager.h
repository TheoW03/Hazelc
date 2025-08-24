#include <vector>
#include <optional>
#include <Frontend/lexxer.h>
#include <iostream>

std::optional<Tokens> match_and_remove(TokenType token_type, std::vector<Tokens> &tokens);
std::optional<Tokens> match_and_remove(std::vector<TokenType> token, std::vector<Tokens> &tokens);
Tokens peek(std::vector<Tokens> &tokens);
bool look_ahead(TokenType t, std::vector<Tokens> &tokens);
bool look_ahead(std::vector<TokenType> token, std::vector<Tokens> &tokens);