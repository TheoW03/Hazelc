#include <optimization/ResolveImports.h>

Imports::Imports() {}
void Imports::Visit(ProgramNode *node)
{

    auto mainModule = node->getMainModule().value();
    this->used_modules.insert(std::make_pair(mainModule->name.value, mainModule));
    this->avail_modules = node->avail_modules;
    mainModule->Accept(this);
    for (const auto &[key, current_module] : this->used_modules)
    {
        std::cout << "imported path: " << current_module->name.value << std::endl;
    }
}
void Imports::Visit(ModuleNode *node)
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