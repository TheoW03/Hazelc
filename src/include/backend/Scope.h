// #include <map>
// #include <iostream>
// #include <optional>
// #include <backend/CompilerUtil.h>
// #include <Frontend/Token.h>

// #ifndef PROGRAM_SCOPE_H
// #define PROGRAM_SCOPE_H
// class ProgramScope
// {
// private:
//     Function currentFunction;
//     std::map<std::string, Function> local_functions;

// public:
//     std::map<std::string, Function> global_functions;
//     std::stack<Function> functions;

//     ProgramScope();
//     ProgramScope(std::map<std::string, Function> global_functions, std::stack<Function> functions);
//     std::optional<Function> get_function(std::string name);

//     Function get_current_function();

//     Function set_current_function();
//     std::optional<int> addLocal(Tokens name, Function function);
// };

// #endif
