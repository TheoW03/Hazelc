#include <Frontend/Token.h>
#include <error.h>

void error(std::string message, Tokens error_token)
{
    std::cout << "hazelc: " << message << " in file  " << error_token.file_name << " on line " << error_token.line_num << std::endl;
    exit(EXIT_FAILURE);
}

void error(std::string message, NodeLocation error_token)
{
    std::cout << "hazelc: " << message << " in file  " << error_token.file_name << " on line " << error_token.line_number << std::endl;
    exit(EXIT_FAILURE);
}
void error(std::string message)
{
    std::cout << "hazelc: " << "unknown option \"" << message << " run -h or --help for a list of commands" << std::endl;
    exit(EXIT_FAILURE);
}