#include <backend/CompilerUtil.h>
Function CompilerContext::get_function(Tokens name)
{
    return func_map[name.value];
}
void CompilerContext::add_function(Tokens name, Function f)
{
    this->func_map.insert(std::make_pair(name.value, f));
}