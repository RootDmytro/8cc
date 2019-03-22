// node.h

#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

enum {
    AST_LITERAL = 256,
    AST_LVAR,
    AST_GVAR,
    AST_TYPEDEF,
    AST_FUNCALL, // 260
    AST_FUNCPTR_CALL,
    AST_FUNCDESG,
    AST_FUNC,
    AST_DECL,
    AST_INIT,
    AST_CONV,
    AST_ADDR,
    AST_DEREF,
    AST_IF,
    AST_TERNARY, // 270
    AST_DEFAULT,
    AST_RETURN,
    AST_COMPOUND_STMT,
    AST_STRUCT_REF,
    AST_GOTO,
    AST_COMPUTED_GOTO,
    AST_LABEL,
    OP_SIZEOF,
    OP_CAST,
    OP_SHR, // 280
    OP_SHL,
    OP_A_SHR,
    OP_A_SHL,
    OP_PRE_INC,
    OP_PRE_DEC,
    OP_POST_INC,
    OP_POST_DEC,
    OP_LABEL_ADDR,
#define op(name, _) name,
#define keyword(name, x, y) name,
#include "keyword.inc"
#undef keyword
#undef op
};

typedef struct Node Node;
typedef struct SourceLoc SourceLoc;
typedef struct Type Type;
typedef struct Vector Vector;


Node *ast_new(int kind);


#include "props.h"

#define AST_RW(type, name) PROP_RW(Node, ast, type, name)
#define AST_RO(type, name) PROP_RO(Node, ast, type, name)

#include "node_p.h"

#undef AST_RW
#undef AST_RO


#endif // NODE_H_INCLUDED
