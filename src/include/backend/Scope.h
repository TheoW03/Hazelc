#include <map>
#include <iostream>
#include <optional>
#include <backend/CompilerUtil.h>
#include <Frontend/Token.h>

#ifndef COMPILED_MODULE_CLASS_H
#define COMPILED_MODULE_CLASS_H
class CompiledModuleClass
{
private:
    CompiledModule compiled_module;
    Function current_function;

public:
    CompiledModuleClass();
    CompiledModuleClass(CompiledModule CompiledModule);
    std::optional<Function> get_function(Tokens name);
    std::optional<Function> get_exported_function(Tokens name);
    std::vector<Tokens> get_import_names();
    Function get_current_function();
    Function set_current_function();
};
#endif

#ifndef PROGRAM_SCOPE_H
#define PROGRAM_SCOPE_H
class ProgramScope
{
private:
    CompiledModuleClass current_module;
    std::map<std::string, Function> local_functions;

public:
    std::map<std::string, CompiledModuleClass> modules;
    ProgramScope();
    ProgramScope(std::map<std::string, CompiledModuleClass> modules);
    std::optional<Function> get_global_function(Tokens name);
    void set_current(Tokens name);
    Function get_function(Tokens name);
    Function get_current_function();
    Function set_current_function();
    std::optional<int> addLocal(Tokens name, Function function);
};

#endif
