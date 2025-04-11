#include <Frontend/lexxer.h>
#include <Frontend/parser.h>
#include <backend/Compiler.h>
#include <fstream>
#include <cstdlib>
#include <cli.h>
#include <stdexcept>
#include <visitor.h>
#include <optimization/TreeShake.h>
#include <filesystem>
#include <Frontend/SemanticCheckScopes.h>
#include <optimization/ConstantFolding.h>

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

void runPasses(std::shared_ptr<ProgramNode> node, Output cli)
{
    // This runs intermediate passes
    // that take care of error checking and basic optimization if needed

    std::cout << "hazelc: constant folding" << std::endl;
    ConstantFoldingVisitor *s2 = new ConstantFoldingVisitor;
    node->Accept(s2);

    // todo: dont forget in the documentation
    //  to much that you dont ave access to improted modules
    //  imports
    std::cout << "hazelc: semantic analysis" << std::endl;
    SemanticGlobalScopeVisitor *semantic =
        new SemanticGlobalScopeVisitor;
    node->Accept(semantic);
    SemanticLocalScopeVisitor *semantic_local =
        new SemanticLocalScopeVisitor(semantic->modules);
    node->Accept(semantic_local);
    std::cout << "" << std::endl;

    ResolveRecursiveModules *recureivemod = new ResolveRecursiveModules;
    std::cout << "hazelc: checking recursive imports" << std::endl;
    node->Accept(recureivemod);
    auto mainModule = node->getMainModule();
    if (mainModule.has_value())
    {
        TreeShake *imporrts = new TreeShake;
        node->Accept(imporrts);
        std::cout << "hazelc: Treeshake" << std::endl;
    }
}

int Init(std::vector<std::string> args)
{

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
    runPasses(modules, cli);

    // SemanticAnalysisTopLevel seman_analy;
    InitCompiler(cli, modules);
    std::cout << "" << std::endl;
    std::cout << "hazelc: \033[32mSuccessfully Compiled \033[0m" << std::endl;
    return EXIT_SUCCESS;
}