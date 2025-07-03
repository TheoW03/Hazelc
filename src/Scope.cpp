// #include <backend/Scope.h>

// /**
//  * ProgramScope represents the whole scope of the program
//  * and control flow. that "CompiledContext" was originally doing
//  */
// ProgramScope::ProgramScope()
// {
// }
// ProgramScope::ProgramScope(std::map<std::string, Function> global_functions, std::stack<Function> functions)
// {
//     this->global_functions = global_functions;
//     this->functions = functions;
// }

// std::optional<Function> ProgramScope::get_function(std::string name)
// {
//     if (this->global_functions.find(name) != this->global_functions.end())
//     {
//         return this->global_functions[name];
//     }
//     if (this->local_functions.find(name) != this->local_functions.end())
//     {
//         return this->local_functions[name];
//     }
//     return {};
// }

// Function ProgramScope::get_current_function()
// {
//     return this->currentFunction;
// }

// Function ProgramScope::set_current_function()
// {
//     this->currentFunction = this->functions.top();
//     this->functions.pop();
//     return this->currentFunction;

//     // return current_module.set_current_function();
// }
// std::optional<int> ProgramScope::addLocal(Tokens name, Function function)
// {
//     if (this->local_functions.find(name.value) != this->local_functions.end())
//     {
//         local_functions[name.value] = function;
//         return {};
//     }
//     else
//     {
//         local_functions.insert(std::make_pair(name.value, function));
//         return 1;
//     }
// }
