#include <vector>
#include <iostream>
#include <Frontend/Token.h>

std::vector<Tokens> lexxer(std::vector<std::string> lines, std::string file_name);
void print_tokens(std::vector<Tokens> tokens);