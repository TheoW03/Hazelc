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
};

#endif
Output parse_cmd(std::vector<Option> parsed_options, std::vector<std::string> args);