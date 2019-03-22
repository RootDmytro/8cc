// parse.h

#ifndef PARSE_H_INCLUDED
#define PARSE_H_INCLUDED

void parse_init(void);

char *make_tempname(void);
char *make_label(void);
bool is_inttype(Type *ty);
bool is_flotype(Type *ty);
void *make_pair(void *first, void *second);
int eval_intexpr(Node *node, Node **addr);
Node *read_expr(void);
Vector *read_toplevels(void);
char *fullpath(char *path);

#endif // PARSE_H_INCLUDED
