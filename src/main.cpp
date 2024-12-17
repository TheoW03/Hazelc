#include <iostream>
#include <Frontend/lexxer.h>
#include <Frontend/parser.h>
#include <fstream>
#include <cstdlib>

int main(int argc, char const *argv[])
{
    /* code */
    std::string c = "2";
    unsigned int b = std::stoul(c, nullptr, 10);
    std::cout << b << std::endl;

    std::ifstream file("test.txt");
    std::string str;
    std::vector<std::string> lines;
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }
    auto a = lexxer(lines);
    std::cout << "" << std::endl;
    print_tokens(a);
    parse_node(a);
    std::cout
        << "" << std::endl;

    std::cout << "Successfully Compiled" << std::endl;
    return 0;
}
