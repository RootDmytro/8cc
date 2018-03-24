// error.h

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <stdbool.h>
#include <stdnoreturn.h>


typedef struct Token Token;

extern bool enable_warning;
extern bool dumpstack;
extern bool dumpsource;
extern bool warning_is_error;

#define STR2(x) #x
#define STR(x) STR2(x)
#define error(...)       errorf(__FILE__ ":" STR(__LINE__), NULL, __VA_ARGS__)
#define errort(tok, ...) errorf(__FILE__ ":" STR(__LINE__), tok_position_string(tok), __VA_ARGS__)
#define warn(...)        warnf(__FILE__ ":" STR(__LINE__), NULL, __VA_ARGS__)
#define warnt(tok, ...)  warnf(__FILE__ ":" STR(__LINE__), tok_position_string(tok), __VA_ARGS__)
#define errorp(p, ...)   errorf(__FILE__ ":" STR(__LINE__), pos_string(&p), __VA_ARGS__)
#define warnp(p, ...)    warnf(__FILE__ ":" STR(__LINE__), pos_string(&p), __VA_ARGS__)

noreturn void errorf(char *line, char *pos, char *fmt, ...);
void warnf(char *line, char *pos, char *fmt, ...);
char *tok_position_string(Token *tok);

#endif // ERROR_H_INCLUDED
