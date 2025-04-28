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

// struct T
// {
//     int a;
//     bool operator<(const struct T &t) const
//     {
//         return t.a < a;
//     }
//     bool operator<=(const struct T &t) const
//     {
//         return t.a <= a;
//     }
//     bool operator>=(const struct T &t) const
//     {
//         return t.a >= a;
//     }
//     bool operator>(const struct T &t) const
//     {
//         return t.a >= a;
//     }
//     bool operator==(const T &other) const
//     {
//         return other.a == a;
//     }
// };
int main(int argc, char *argv[])
{
    std::vector<std::string> args;
    // std::map<T, int> c;
    // struct T t = {1};
    // c.insert(std::make_pair(t, 1));
    // std::cout << c[{1}] << std::endl;
    for (int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }
    return Init(args);
}
