#include <Frontend/lexxer.h>
#include <Frontend/parser.h>
#include <backend/Compiler.h>
#include <fstream>
#include <cstdlib>
#include <cli.h>
#include <stdexcept>
#include <visitor.h>
#include <Demoduerlization.h>

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

std::vector<Tokens> lex_file(Output cli)
{
    std::vector<Tokens> tokens;
    size_t in = 0;
    for (int i = 0; i < cli.files.size(); i++)
    {
        if (std::filesystem::is_directory(cli.files[i]))
        {
            for (const auto &entry : std::filesystem::directory_iterator(cli.files[i]))
            {
                if (entry.path().extension() == ".hz")
                {
                    std::vector<std::string> lines;
                    std::ifstream file(entry.path());
                    get_files(file, lines);
                    std::cout << "hazelc input file: " << entry.path() << std::endl;
                    auto c = lexxer(lines, entry.path());
                    tokens.insert(tokens.begin(), c.begin(), c.end());
                    file.close();

                    in++;
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
            std::vector<std::string> lines;
            get_files(file, lines);
            auto c = lexxer(lines, cli.files[i]);
            tokens.insert(tokens.begin(), c.begin(), c.end());

            file.close();
            in++;
        }
    }
    tokens.push_back({"EOF", TokenType::EndOfFile});

    return tokens;
}
std::vector<std::string> get_lines(Output cli)
{
    std::vector<std::string> lines;
    size_t in = 0;
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
                    in++;
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
            in++;
        }
    }
    std::cout << "" << std::endl;
    std::cout << "hazelc total input files/directories: " << (cli.files.size()) << std::endl;
    std::cout << "hazelc total number of files: " << in << std::endl;

    return lines;
}

void runPasses(std::shared_ptr<ProgramNode> node, Output cli)
{
    // This runs intermediate passes
    // that take care of error checking and basic optimization if needed

    // it uses the visitor pattern for tree traversal.
    // but what it does is it uses virtual impls the method in an abstract class
    // so it will only visit the nodes that have the method overriden in that class
    // handy trick i have stolen from my other compiler project :P

    node->Accept(std::make_shared<ResolveRecursiveModules>().get());
    std::cout << "hazelc: resolved recursive imports" << std::endl;
    // SemanticGlobalScopeVisitor *semantic =
    // new SemanticGlobalScopeVisitor;
    auto semantic = std::make_shared<SemanticGlobalScopeVisitor>();
    node->Accept(semantic.get());
    std::cout << "hazelc: resolved Global Scope" << std::endl;
    auto semantic_local = std::make_shared<SemanticLocalScopeVisitor>(semantic->modules);
    node->Accept(semantic_local.get());
    std::cout << "hazelc: resolved Local scope" << std::endl;
    std::cout << "" << std::endl;
    auto typechecker = std::make_shared<TypeCheckerVistor>(IntermediateScope(semantic->modules));
    node->Accept(typechecker.get());
    std::cout << "hazelc: checking types" << std::endl; // /

    node->Accept(std::make_shared<ConstantFoldingVisitor>().get()); // aa

    std::cout << "hazelc: constant folding" << std::endl;
    auto mainModule = node->getMainModule();

    if (mainModule.has_value())
    {
        std::shared_ptr<TreeShake> shake_imports = std::make_shared<TreeShake>();
        node->Accept(shake_imports.get());
        std::cout << "hazelc: Treeshake" << std::endl;
    }
    auto demoddlarize = std::make_shared<DemodularizedVisitor>(IntermediateScope(semantic->modules));
    node->Accept(demoddlarize.get());

    std::cout << "hazelc: demodularize" << std::endl; //
    InitCompiler(cli, std::make_shared<DemoduarlizedProgramNode>(demoddlarize->program));
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
    // auto lines = get_lines(cli);
    // std::cout << "" << std::endl;

    auto tokens = lex_file(cli);

    std::cout << "hazelc: lexxed" << std::endl;
    std::cout << "" << std::endl;
    if (cli.print_tokens == 1)
        print_tokens(tokens);
    auto modules = parse_node(tokens);
    std::cout << modules->avail_modules.size() << std::endl;
    // for (int i = 0; i < modules->)

    std::cout << "hazelc: parsed" << std::endl;
    std::cout << "" << std::endl;
    runPasses(modules, cli);

    // SemanticAnalysisTopLevel seman_analy;
    // InitCompiler(cli, modules);
    std::cout << "" << std::endl;
    std::cout << "hazelc: \033[32mSuccessfully Compiled \033[0m" << std::endl;
    return EXIT_SUCCESS;
}