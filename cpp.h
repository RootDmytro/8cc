// cpp.h

#ifndef CPP_H_INCLUDED
#define CPP_H_INCLUDED

typedef struct Token Token;


void cpp_init(void);

void read_from_string(char *buf);
void expect_newline(void);
void add_include_path(char *path);
Token *peek_token(void);
Token *read_token(void);

#endif // CPP_H_INCLUDED
