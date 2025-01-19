#include <iostream>
#include <Frontend/lexxer.h>
#include <Frontend/parser.h>
#include <backend/Compiler.h>
#include <fstream>
#include <cstdlib>
#include <cli.h>
#include <stdexcept>

int main(int argc, char *argv[])
{
    /* code */

    std::ifstream file(argv[1]);
    std::string str;
    std::vector<std::string> lines;
    std::vector<std::string> args;

    for (int i = 0; i < argc; i++)
    {
        args.push_back(argv[i]);
    }
    parse_cmd({}, args);
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }
    auto a = lexxer(lines);
    std::cout << "lexxed" << std::endl;

    // std::cout << "" << std::endl;
    auto p = parse_node(a);
    std::cout << "parsed" << std::endl;
    // std::cout << "" << std::endl;
    // std::cout << "parsed expr: " << p.value()->to_string() << std::endl;
    // std::cout << "" << std::endl;
    InitCompiler("file", p);

    std::cout << "" << std::endl;
    std::cout << "Successfully Compiled" << std::endl;

    return 0;
}
