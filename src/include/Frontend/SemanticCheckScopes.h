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
    std::map<std::string, std::shared_ptr<FunctionRefNode>> functions;
    std::map<std::string, std::shared_ptr<FunctionRefNode>> exported_functions;
    std::vector<Tokens> imports;
    // std::shared_ptr<ModuleNode> module;
    // std::map<std::string, std::vector<SemanticFunction>> functions;
};

#endif
#ifndef INTERMEDIATE_SCOPE_H
#define INTERMEDIATE_SCOPE_H

class IntermediateScope
{
private:
    std::optional<SemanticModule> getModuleNameByFunction(Tokens name);

public:
    std::map<std::string, SemanticModule> modules;
    std::unordered_map<std::string, std::shared_ptr<FunctionNode>> functions;
    SemanticModule current;
    IntermediateScope();

    IntermediateScope(std::map<std::string, SemanticModule> modules);
    IntermediateScope(std::unordered_map<std::string, std::shared_ptr<FunctionNode>> functions);

    void set_current_module(std::string s);
    std::optional<std::shared_ptr<FunctionRefNode>> get_global_function(Tokens function_name);
    std::optional<std::shared_ptr<FunctionRefNode>> get_global_function_demodularized(std::optional<std::string> function_name);

    std::optional<std::string> get_global_function_hash(Tokens function_name);
};
#endif

#ifndef SEMANTIC_TOP_H
#define SEMANTIC_TOP_H
class SemanticGlobalScopeVisitor : public Visitor
{
public:
    std::map<std::string, std::shared_ptr<ModuleNode>> avail_modules;
    std::map<std::string, std::shared_ptr<FunctionRefNode>> module_functions;
    std::map<std::string, std::shared_ptr<FunctionRefNode>> exported_functions;
    std::map<std::string, SemanticModule> modules;
    std::shared_ptr<ModuleNode> current;
    SemanticModule current_AST_module;
    bool function_exists(Tokens name);
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
    std::map<std::string, std::shared_ptr<FunctionRefNode>> params;

    std::map<std::string, std::shared_ptr<FunctionRefNode>> functions;
};
#endif

#ifndef SEMANTIC_SCOPE_VISTOR_H
#define SEMANTIC_SCOPE_VISTOR_H

class SemanticLocalScopeVisitor : public Visitor
{
private:
    std::vector<FunctionLocalScope> scope;
    std::optional<bool> find_function_global(Tokens name);
    std::optional<bool> find_function_local(Tokens name);
    std::optional<bool> find_function(Tokens name);
    bool function_is_param(Tokens name);

    std::optional<std::shared_ptr<FunctionRefNode>> get_local_function(Tokens name);
    std::optional<std::shared_ptr<FunctionRefNode>> get_function(Tokens name);
    std::optional<std::shared_ptr<FunctionRefNode>> get_global_function(Tokens name);

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
    void Visit(BlockNode *node) override;
};
#endif

#ifndef TYPE_CHECKER_VISITOR_H
#define TYPE_CHECKER_VISITOR_H

class TypeCheckerVistor : public Visitor
{
private:
    void add_local_function(Tokens name, std::shared_ptr<FunctionRefNode> value);

public:
    IntermediateScope modules;
    SemanticModule current_AST_module;
    std::map<std::string, std::shared_ptr<FunctionRefNode>> local_functions;

    TypeCheckerVistor(IntermediateScope modules);

    // void Visit(ProgramNode *node) override;
    // void Visit(ModuleNode *node) override;
    void Visit(FunctionNode *node) override;

    void Visit(ExprNode *node) override;
    void Visit(BooleanExprNode *node) override;
    void Visit(ConditionalNode *node) override;
    void Visit(DemoduarlizedProgramNode *node) override;
};
#endif

#ifndef CHECK_EXPR_TYPE_H
#define CHECK_EXPR_TYPE_H

class CheckExpressionType : public Visitor
{
public:
    std::shared_ptr<Type> match_type;
    IntermediateScope s;
    std::map<std::string, std::shared_ptr<FunctionRefNode>> local_functions;
    CheckExpressionType(std::shared_ptr<Type> match_type, IntermediateScope s, std::map<std::string, std::shared_ptr<FunctionRefNode>> local_functions);
    std::shared_ptr<Type> traverse_type(std::shared_ptr<ASTNode> expr);
};
#endif
