#include <iostream>
#include <Frontend/Token.h>

#ifndef NODE_LOC_H
#define NODE_LOC_H
struct NodeLocation
{
    std::string file_name;
    int line_number;
};
#endif

void error(std::string message, Tokens error_token);
void error(std::string message, NodeLocation error_token);