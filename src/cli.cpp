#include <vector>
#include <iostream>
#include <cli.h>
Output parse_cmd(std::vector<Option> parsed_options, std::vector<std::string> args)
{
    Output o;
    for (int i = 0; i < args.size(); i++)
    {
        if (args[i].at(0) == '-')
        {
            std::string optop = args[i].substr(1, args[i].size());
            std::cout << "options: " << optop << std::endl;
        }
        else
        {
            o.files.push_back(args[i]);
        }
    }
    return o;
}