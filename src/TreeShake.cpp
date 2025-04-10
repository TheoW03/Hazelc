#include <optimization/TreeShake.h>

// this pass impls a "tree-shake" it goes through each module
// creates a data strcuture of the modules that are used
// its only implemented if the user doesnt link along with create an object file

TreeShake::TreeShake() {}
void TreeShake::Visit(ProgramNode *node)
{

    auto mainModule = node->getMainModule().value();
    this->used_modules.insert(std::make_pair(mainModule->name.value, mainModule));
    this->avail_modules = node->avail_modules;
    mainModule->Accept(this);
}
void TreeShake::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->imports.size(); i++)
    {
        auto m = avail_modules[node->imports[i].value];
        if (this->used_modules.find(m->name.value) == this->used_modules.end())
        {
            std::cout << m->name.value << std::endl;
            this->used_modules.insert(std::make_pair(m->name.value, m));
            m->Accept(this);
        }
    }
}