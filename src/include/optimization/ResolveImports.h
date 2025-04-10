#include <visitor.h>

#ifndef IMPORTS_H
#define IMPORTS_H

class Imports : public Visitor
{

public:
    std::map<std::string, std::shared_ptr<ModuleNode>> used_modules;
    std::map<std::string, std::shared_ptr<ModuleNode>> avail_modules;
    Imports();
    void Visit(ProgramNode *node) override;
    void Visit(ModuleNode *node) override;
};
#endif