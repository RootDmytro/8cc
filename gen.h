// gen.h

#ifndef GEN_H_INCLUDED
#define GEN_H_INCLUDED

struct _IO_FILE;
typedef struct _IO_FILE FILE;
typedef struct Node Node;

void set_output_file(FILE *fp);
void close_output_file(void);
void emit_toplevel(Node *v);

#endif // GEN_H_INCLUDED
