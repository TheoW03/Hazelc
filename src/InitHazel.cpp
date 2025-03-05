#include <Frontend/lexxer.h>
#include <Frontend/parser.h>
#include <backend/Compiler.h>
#include <fstream>
#include <cstdlib>
#include <cli.h>
#include <stdexcept>
#include <visitor.h>
#include <filesystem>
void get_files(std::ifstream &file, std::vector<std::string> &lines)
{
    std::string str;
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }
}
std::vector<std::string> get_lines(Output cli)
{
    std::vector<std::string> lines;

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
                    std::cout << "hazelc input file: " << entry.path() << std::endl;
                    file.close();
                }
            }
        }
        else
        {
            std::ifstream file(cli.files[i]);
            if (!file)
            {
                std::cout << "hazelc: file \"" << cli.files[i] << "\"doesnt exist" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::cout << "hazelc input file: " << cli.files[i] << std::endl;
            get_files(file, lines);
            file.close();
        }
    }
    return lines;
}
int Init(std::vector<std::string> args)
{

    /* code */

    auto cli = parse_cmd(args);
    if (cli.files.size() == 0)
    {
        std::cout << "hazelc: lack of files" << std::endl;
        std::cout << "compilation termianted" << std::endl;
        return EXIT_FAILURE;
    }
    auto lines = get_lines(cli);
    std::cout << "hazelc total input files: " << (cli.files.size() + 1) << std::endl;
    std::cout << "" << std::endl;

    auto tokens = lexxer(lines);
    std::cout << "hazelc: lexxed" << std::endl;
    std::cout << "" << std::endl;
    if (cli.print_tokens == 1)
        print_tokens(tokens);
    auto modules = parse_node(tokens);
    std::cout << "hazelc: parsed" << std::endl;
    std::cout << "" << std::endl;
    SemanticAnalysisTopLevel *s = new SemanticAnalysisTopLevel;
    modules->Accept(s);
    std::cout << "hazelc: semantic analysis" << std::endl;
    std::cout << "" << std::endl;

    // SemanticAnalysisTopLevel seman_analy;
    InitCompiler(cli, modules);
    std::cout << "" << std::endl;
    std::cout << "hazelc: \033[32mSuccessfully Compiled \033[0m" << std::endl;
    return EXIT_SUCCESS;
}