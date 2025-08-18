#include <vector>
#include <iostream>

#ifndef OP_H
#define OP_H
struct Option
{
    std::string Name;
    std::string help_text;
};

#endif

#ifndef OPTIMIZE_LEVEL_H
#define OPTIMIZE_LEVEL_H
enum OptimizeLevel
{
    No_Optimize,
    level_1,
    level_2,
    level_3,
};
#endif

#ifndef FILE_TYPE_H
#define FILE_TYPE_H
enum FileType
{
    Assembly_File,
    Object_file,
    Ir_file,
    exe_file
};
#endif
#ifndef OUT_H
#define OUT_H
struct Output
{
    std::vector<std::string> files;
    std::vector<std::string> output_files;
    int print_llvm;
    int print_tokens;
    OptimizeLevel optimze_level;
    FileType gen_file;
};

#endif
Output parse_cmd(std::vector<std::string> args);