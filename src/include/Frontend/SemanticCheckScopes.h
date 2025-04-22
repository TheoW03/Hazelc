#include <visitor.h>

#ifndef SEMANTUC_FUCN
#define SEMANTUC_FUCN
struct SemanticFunction
{
    std::vector<std::shared_ptr<Type>> paremeters;
    std::shared_ptr<Type> returnType;
};

#endif

#ifndef SEMANTIC_MODULE_H
#define SEMANTIC_MODULE_H

struct SemanticModule
{
    Tokens name;
    std::map<std::string, FunctionNode *> functions;
    std::map<std::string, FunctionNode *> exported_functions;
    std::vector<Tokens> imports;
    // std::shared_ptr<ModuleNode> module;
    // std::map<std::string, std::vector<SemanticFunction>> functions;
};

#endif

#ifndef SEMANTIC_TOP_H
#define SEMANTIC_TOP_H
class SemanticGlobalScopeVisitor : public Visitor
{
public:
    std::map<std::string, std::shared_ptr<ModuleNode>> avail_modules;
    std::map<std::string, FunctionNode *> module_functions;
    std::map<std::string, FunctionNode *> exported_functions;
    std::map<std::string, SemanticModule> modules;
    std::shared_ptr<ModuleNode> current;
    SemanticModule current_AST_module;
    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ModuleNode *node) override;
    void Visit(ReturnNode *node) override;
    void Visit(FunctionCallNode *node) override;
    void Visit(ProgramNode *node) override;
};
#endif

#ifndef FUNCTION_LOCAL_SCOPE_H
#define FUNCTION_LOCAL_SCOPE_H
struct FunctionLocalScope
{
    std::map<std::string, FunctionNode *> functions;
};
#endif

#ifndef SEMANTIC_SCOPE_VISTOR_H
#define SEMANTIC_SCOPE_VISTOR_H

class SemanticLocalScopeVisitor : public Visitor
{
private:
    std::vector<FunctionLocalScope> scope;
    std::optional<FastLookup> find_function_global(Tokens name);
    std::optional<FastLookup> find_function_local(Tokens name);
    std::optional<FastLookup> find_function(Tokens name);

    std::optional<FunctionNode *> get_local_function(Tokens name);
    std::optional<FunctionNode *> get_function(Tokens name);
    std::optional<FunctionNode *> get_global_function(Tokens name);

    std::map<std::string, SemanticModule> modules;
    SemanticModule current_AST_module;

public:
    SemanticLocalScopeVisitor(std::map<std::string, SemanticModule> modules);

    void Visit(ASTNode *node) override;
    void Visit(FunctionNode *node) override;
    void Visit(ModuleNode *node) override;
    void Visit(ReturnNode *node) override;
    void Visit(FunctionCallNode *node) override;
    void Visit(ProgramNode *node) override;
    void Visit(ExprNode *node) override;
    void Visit(BooleanExprNode *node) override;

    void Visit(ConditionalNode *node) override;
    void Visit(BranchNode *node) override;
};
#endif
