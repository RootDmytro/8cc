// debug.h

#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

typedef struct Type Type;
typedef struct Node Node;
typedef struct Token Token;

char *ty2s(Type *ty);
char *node2s(Node *node);
char *tok2s(Token *tok);

#endif // DEBUG_H_INCLUDED
