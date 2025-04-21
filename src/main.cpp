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
enum Foo
{
    VariantFoo = 1,
    Variant2 = 0
};

enum Bar
{
    Variant1 = Foo::VariantFoo,
    Variants
};
int main(int argc, char *argv[])
{
    std::vector<std::string> args;

    for (int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }
    return Init(args);
}
