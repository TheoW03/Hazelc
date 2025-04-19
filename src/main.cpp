#include <iostream>
#include <Frontend/lexxer.h>
#include <Frontend/parser.h>
#include <backend/Compiler.h>
#include <fstream>
#include <cstdlib>
#include <cli.h>
#include <stdexcept>
#include <filesystem>
#include <InitHazel.h>

int main(int argc, char *argv[])
{
    /* code */
    std::vector<std::string>
        args;

    for (int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }
    return Init(args);
}
