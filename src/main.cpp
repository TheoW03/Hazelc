#include <iostream>
#include <Frontend/lexxer.h>
#include <fstream>

int main(int argc, char const *argv[])
{
    /* code */
    std::ifstream file("read.txt");
    std::string str;
    std::vector<std::string> lines;
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }
    auto a = lexxer(lines);
    print_tokens(a);
    std::cout << "Successfully compiled" << std::endl;
    return 0;
}
