#include <optimization/TreeShake.h>
#include <error.h>

// I made this its own seperate class but what it does now is that it
// runs a pass to check if recursive imports using this algorithm
// https://en.wikipedia.org/wiki/Cycle_(graph_theory)#Algorithm

ResolveRecursiveModules::ResolveRecursiveModules()
{
}

void ResolveRecursiveModules::Visit(ProgramNode *node)
{
    this->avail_modules = node->avail_modules;
    for (const auto &[key, current_module] : node->avail_modules)
    {

        this->visited_modules.insert(current_module->name.value);
        current_module->Accept(this);
        this->visited_modules.erase(current_module->name.value);
        this->used_modules.clear();
    }
    std::cout << "hazelc: Resolved Recursive Imports" << std::endl;
}

void ResolveRecursiveModules::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->imports.size(); i++)
    {
        if (this->avail_modules.find(node->imports[i].value) == this->avail_modules.end())
        {
            error("invalid import, " + node->imports[i].value, node->imports[i]);
        }
        auto m = avail_modules[node->imports[i].value];

        if (visited_modules.find(m->name.value) != visited_modules.end())
        {
            error("circular import, " + node->name.value + "depends on module " + m->name.value, node->name);
        }
        if (this->used_modules.find(m->name.value) == this->used_modules.end())
        {
            this->used_modules.insert(m->name.value);
            this->visited_modules.insert(m->name.value);
            m->Accept(this);
            this->visited_modules.erase(m->name.value);
        }
    }
}
