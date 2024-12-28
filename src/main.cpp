#include <iostream>
#include <Frontend/lexxer.h>
#include <Frontend/parser.h>
#include <backend/Compiler.h>
#include <fstream>
#include <cstdlib>

int main(int argc, char *argv[])
{
    /* code */

    std::ifstream file("test.txt");
    std::string str;
    std::vector<std::string> lines;
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }
    auto a = lexxer(lines);
    // std::cout << "" << std::endl;
    // print_tokens(a);
    auto p = parse_node(a);
    // std::cout << "" << std::endl;
    // std::cout << "parsed expr: " << p.value()->to_string() << std::endl;
    // std::cout << "" << std::endl;
    InitCompiler("file");
    std::cout << "Successfully Compiled" << std::endl;
    return 0;
}
