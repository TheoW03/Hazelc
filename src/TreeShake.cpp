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
}
void TreeShake::Visit(ModuleNode *node)
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
            // if a module is already visited we dont visit
            //(supposed to resolve if main depends on foo and bar and foo depends on bar)
            this->used_modules.insert(std::make_pair(m->name.value, m));
            this->visited_modules.insert(m->name.value);
            m->Accept(this);
            this->visited_modules.erase(m->name.value);
        }
    }
}