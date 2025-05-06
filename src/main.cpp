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
// // };

// int test_lexxer()
// {
//     std::vector<std::string> lines =
//         {"1+1"};
//     auto result = lexxer(lines);

//     std::vector<Tokens> expected = {{"1", TokenType::BaseTenDigit, 0},
//                                     {"", TokenType::Addition, 0},
//                                     {"1", TokenType::BaseTenDigit, 0},
//                                     {"", TokenType::EndOfFile, 0}};

//     if (expected.size() != result.size())
//     {
//         std::cout << "test failed: " << result.size() << std::endl;
//         exit(EXIT_FAILURE);
//     }
//     for (int i = 0; i < expected.size(); i++)
//     {
//         if (result[i] != expected[i])
//         {
//             std::cout << "test failed: " << result[i].value << std::endl;
//             exit(EXIT_FAILURE);
//         }
//     }
// }
int main(int argc, char *argv[])
{
    std::vector<std::string> args;
    // std::map<T, int> c;
    // struct T t = {1};
    // c.insert(std::make_pair(t, 1));
    // std::cout << c[{1}] << std::endl;
    // test_lexxer();
    for (int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);
    }
    return Init(args);
}
