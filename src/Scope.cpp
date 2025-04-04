#include <backend/Scope.h>

/**
 * ProgramScope represents the whole scope of the program
 * and control flow. that "CompiledCOntext" was originally doing
 */
ProgramScope::ProgramScope()
{
}
ProgramScope::ProgramScope(std::map<std::string, CompiledModuleClass> modules)
{
    this->modules = modules;
}

void ProgramScope::set_current(Tokens name)
{
    this->current_module = modules[name.value];
}
Function ProgramScope::get_function(Tokens name)
{
    auto global = this->get_global_function(name);
    if (global.has_value())
    {
        return global.value();
    }
    else
    {
        return local_functions[name.value];
    }
}

std::optional<Function> ProgramScope::get_global_function(Tokens name)
{
    if (!this->current_module.get_function(name).has_value())
    {
        auto import_list = this->current_module.get_import_names();
        for (int i = 0; i < import_list.size(); i++)
        {
            if (modules[import_list[i].value].get_exported_function(name).has_value())
            {
                return modules[import_list[i].value].get_exported_function(name).value();
            }
        }
        return {};
    }
    return this->current_module.get_function(name);
}
Function ProgramScope::get_current_function()
{
    return current_module.get_current_function();
}

Function ProgramScope::set_current_function()
{
    return current_module.set_current_function();
}

std::optional<int> ProgramScope::addLocal(Tokens name, Function function)
{
    if (this->local_functions.find(name.value) != this->local_functions.end())
    {
        local_functions[name.value] = function;
        std::cout << "aaa" << std::endl;
        return {};
    }
    else
    {
        local_functions.insert(std::make_pair(name.value, function));
        return 1;
    }
}
CompiledModuleClass::CompiledModuleClass()
{
}

CompiledModuleClass::CompiledModuleClass(CompiledModule CompiledModule)
{
    this->compiled_module = CompiledModule;
}

std::optional<Function> CompiledModuleClass::get_function(Tokens name)
{
    if (this->compiled_module.func_map.find(name.value) == this->compiled_module.func_map.end())
    {
        return {};
    }
    return this->compiled_module.func_map[name.value];
}

std::vector<Tokens> CompiledModuleClass::get_import_names()
{
    return compiled_module.imports;
}

std::optional<Function> CompiledModuleClass::get_exported_function(Tokens name)
{

    if (this->compiled_module.export_function.find(name.value) == this->compiled_module.export_function.end())
    {
        return {};
    }
    return this->compiled_module.export_function[name.value];
}
Function CompiledModuleClass::get_current_function()
{
    return this->current_function;
}

Function CompiledModuleClass::set_current_function()
{
    this->current_function = this->compiled_module.functions.front();
    compiled_module.functions.pop();
    return this->current_function;
}
