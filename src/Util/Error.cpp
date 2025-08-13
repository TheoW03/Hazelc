#include <Frontend/Token.h>
#include <error.h>
#define RED "\033[31m"
void error(std::string message, Tokens error_token)
{
    std::cout << "\033[31mhazelc: " << message << " in file " << error_token.file_name << " on line " << error_token.line_num << "\033[30m" << std::endl;
    exit(EXIT_FAILURE);
}

void error(std::string message, NodeLocation error_token)
{
    std::cout << "\033[31mhazelc: " << message << " in file " << error_token.file_name << " on line " << error_token.line_number << "\033[30m" << std::endl;
    exit(EXIT_FAILURE);
}
void error(std::string message)
{
    std::cout << "\033[31mhazelc: " << "unknown option \"" << message << " run -h or --help for a list of commands" << "\033[30m" << std::endl;
    exit(EXIT_FAILURE);
}
void status(std::string message)
{
    std::cout << "\033[31mhazelc: " << message << "\033[30m" << std::endl;
}