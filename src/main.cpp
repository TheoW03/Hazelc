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

    std::vector<std::string> lines;
    std::vector<std::string> args;

    for (int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }
    auto cli = parse_cmd(args);
    std::string str;
    std::ifstream file(cli.files[0]);
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }
    auto a = lexxer(lines);
    std::cout << "lexxed" << std::endl;
    if (cli.print_tokens == 1)
        print_tokens(a);

    // std::cout << "" << std::endl;
    auto p = parse_node(a);
    std::cout << "parsed" << std::endl;
    // std::cout << "" << std::endl;
    // std::cout << "parsed expr: " << p.value()->to_string() << std::endl;
    // std::cout << "" << std::endl;
    InitCompiler(cli, p);

    std::cout << "" << std::endl;
    std::cout << "\033[32mSuccessfully Compiled \033[0m" << std::endl;

    return 0;
}
