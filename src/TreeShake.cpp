#include <optimization/TreeShake.h>

// this pass impls a "tree-shake" for modules only for functions the LLVM has to be the one todo this.
// It goes through each module
// creates a data strcuture of the modules that are used
// its only implemented if the user doesnt link along with create an object file

TreeShake::TreeShake() {}
void TreeShake::Visit(ProgramNode *node)
{

    auto mainModule = node->getMainModule().value();
    this->used_modules.insert(std::make_pair(mainModule->name.value, mainModule));
    this->avail_modules = node->avail_modules;
    this->visited_modules.insert(mainModule->name.value);

    mainModule->Accept(this);
    node->avail_modules = this->used_modules;
}
void TreeShake::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->imports.size(); i++)
    {

        auto m = avail_modules[node->imports[i].value];

        if (this->used_modules.find(m->name.value) == this->used_modules.end())
        {
            // if a module is already visited we dont visit
            //(supposed to resolve if main depends on foo and bar and foo depends on bar)
            this->used_modules.insert(std::make_pair(m->name.value, m));
            this->visited_modules.insert(m->name.value);
            m->Accept(this);
            this->visited_modules.erase(m->name.value);
        }
    }
}

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
}

void ResolveRecursiveModules::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->imports.size(); i++)
    {
        auto m = avail_modules[node->imports[i].value];
        if (visited_modules.find(m->name.value) != visited_modules.end())
        {
            std::cout << "hazelc: cicurlar import module " << node->name.value << " depends on module " << m->name.value << std::endl;
            exit(EXIT_FAILURE);
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
