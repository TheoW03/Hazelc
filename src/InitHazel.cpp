#include <Frontend/lexxer.h>
#include <Frontend/parser.h>
#include <backend/Compiler.h>
#include <fstream>
#include <cstdlib>
#include <cli.h>
#include <stdexcept>
#include <filesystem>
void get_files(std::ifstream &file, std::vector<std::string> &lines)
{
    std::string str;
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }
}
int Init(std::vector<std::string> args)
{

    /* code */

    std::vector<std::string> lines;

    auto cli = parse_cmd(args);
    for (int i = 0; i < cli.files.size(); i++)
    {
        if (std::filesystem::is_directory(cli.files[i]))
        {
            for (const auto &entry : std::filesystem::directory_iterator(cli.files[i]))
            {
                if (entry.path().extension() == ".hz")
                {
                    std::ifstream file(entry.path());
                    get_files(file, lines);
                    file.close();
                }
            }
        }
        else
        {
            std::ifstream file(cli.files[i]);
            get_files(file, lines);
            file.close();
        }
    }

    auto tokens = lexxer(lines);
    std::cout << "lexxed" << std::endl;
    if (cli.print_tokens == 1)
        print_tokens(tokens);
    auto modules = parse_node(tokens);
    std::cout << "parsed" << std::endl;
    InitCompiler(cli, modules);
    std::cout << "" << std::endl;
}