#include <vector>
#include <iostream>

#ifndef OP_H
#define OP_H
struct Option
{
    std::string Name;
    std::string help_text;
};

#endif

#ifndef OUT_H
#define OUT_H
struct Output
{
    std::vector<std::string> files;
    std::vector<std::string> output_files;
    int print_llvm;
    int print_tokens;
};

#endif
Output parse_cmd(std::vector<std::string> args);