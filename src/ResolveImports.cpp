#include <optimization/ResolveImports.h>

void Imports::Visit(ProgramNode *node)
{

    auto mainModule = node->getMainModule().value();
    this->used_modules.insert(std::make_pair(mainModule->name.value, mainModule));
    mainModule->Accept(this);
    for (const auto &[key, current_module] : this->used_modules)
    {
        std::cout << current_module->name.value << std::endl;
    }
}
void Imports::Visit(ModuleNode *node)
{
    for (int i = 0; i < node->imports.size(); i++)
    {
        auto m = avail_modules[node->imports[i].value];
        if (this->used_modules.find(m->name.value) == this->avail_modules.end())
        {

            this->used_modules.insert(std::make_pair(m->name.value, m));
            m->Accept(this);
        }
    }
}