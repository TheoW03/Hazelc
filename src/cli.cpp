#include <vector>
#include <map>
#include <iostream>
#include <cli.h>
#include <functional>
#include <optional>
struct Parse_F
{
    std::function<void(std::string &, Output &)> function;
    std::string help_text;
    int takes_args;
};
Output parse_cmd(std::vector<std::string> args)
{
    int state = 0;
    Output o;
    /* A function wrapper to a lambda function. */
    std::map<std::string,
             Parse_F>
        option_handlers;
    std::string optop;
    std::map<char, std::string> short_hand;
    short_hand['c'] = "object-file";
    short_hand['o'] = "output-file";
    short_hand['v'] = "version";
    short_hand['h'] = "help";

    option_handlers["output-file"] = {[](std::string &value, Output &o)
                                      {
                                          o.output_files.push_back(value);
                                      },
                                      "output file", 1};
    option_handlers["print-llvm"] = {[](std::string &value, Output &o)
                                     {
                                         o.print_llvm = 1;
                                     },
                                     "prints the llvm IR", 0};
    option_handlers["print-tokens"] = {[](std::string &value, Output &o)
                                       {
                                           o.print_tokens = 1;
                                       },
                                       "prints tokens taken by the lexxer", 0};
    option_handlers["help"] = {[&option_handlers](std::string &value, Output &o)
                               {
                                   std::cout << "hazelc v1.0.0" << std::endl;
                                   std::cout << "" << std::endl;
                                   std::cout << "   hazelc is a LLVM compiler for hazel, a functional paradigm programming langauge." << std::endl;
                                   std::cout << "" << std::endl;
                                   std::cout << "usage: hazelc file.. [options] or hazelc [options] file..." << std::endl;
                                   std::cout << "" << std::endl;

                                   for (const auto &[key, value] : option_handlers)
                                   {
                                       std::cout << "   --" << key << ": " << value.help_text << std::endl; // Access the key
                                       std::cout << "" << std::endl;
                                   }
                               },
                               "cli documentation", 0};
    option_handlers["version"] = {[](std::string &value, Output &o)
                                  {
                                      std::cout << "1.0.0" << std::endl;
                                  },
                                  "prints version", 0};
    option_handlers["object-file"] = {[](std::string &value, Output &o)
                                      {
                                          o.gen_file = FileType::Object_file;
                                      },
                                      "out puts an non linked binary file", 0};

    option_handlers["assembly-file"] = {[](std::string &value, Output &o)
                                        {
                                            o.gen_file = FileType::Assembly_File;
                                        },
                                        "out puts a .s/.asm file", 0};
    option_handlers["ir-file"] = {[](std::string &value, Output &o)
                                  {
                                      o.gen_file = FileType::Ir_file;
                                  },
                                  "out puts an unoptimized ir file", 0};
    option_handlers["optimize"] = {[](std::string &value, Output &o) {

                                   },
                                   "optimization level 0 being the lowest 3 being the heighest", 0};

    // func_c(2017);
    std::optional<Parse_F> current;
    for (int i = 0; i < args.size(); i++)
    {
        if (args[i].at(0) == '-' && args[i].at(1) != '-')
        {
            char short_optop = args[i].at(1);
            if (short_hand.find(short_optop) != short_hand.end())
            {
                current = option_handlers[short_hand[short_optop]];
                if (current.value().takes_args == 0)
                {
                    current.value().function(args[i], o);
                    current = {};
                }
            }
            else
            {
                std::cout << "unknown option \"" << short_optop << "\"" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if (args[i].at(0) == '-' && args[i].at(1) == '-')
        {
            optop = args[i].substr(2, args[i].size());
            if (option_handlers.find(optop) != option_handlers.end())
            {
                current = option_handlers[optop];
                if (current.value().takes_args == 0)
                {
                    current.value().function(args[i], o);
                    current = {};
                }
            }
            else
            {
                std::cout << "unknown option \"" << optop << "\"" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if (current.has_value())
        {
            current.value().function(args[i], o);
        }
        else
        {
            o.files.push_back(args[i]);
        }
    }
    return o;
}
