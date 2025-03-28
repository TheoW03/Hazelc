#include <visitor.h>
#include <Token.h>

#ifndef SEMANTIC_FUNCTUON_H
#define SEMANTIC_FUNCTUON_H
struct SemanticFunction
{
    Tokens name;
    std::shared_ptr<Type> ret_type;
};
#endif

#ifndef SEMANTIC_GLOBAL_SCOPE_H
#define SEMANTIC_GLOBAL_SCOPE_H
class SemanticGlobalScope
{
};
#endif
// #ifndef SEMANTIC_TYPE_H
// #define SEMANTIC_TYPE_H
// class SemanticType
// {
// public:
//     virtual bool canAccept() = 0;
// };
// #endif

// #ifndef SEMANTIC_TYPE_H
// #define SEMANTIC_TYPE_H
// class SemanticType
// {
// public:
//     virtual bool canAccept() = 0;
// };
// #endif

// #ifndef SEMANTIC_TYPE_H
// #define SEMANTIC_TYPE_H
// class SemanticType
// {
// public:
//     virtual bool canAccept() = 0;
// };
// #endif