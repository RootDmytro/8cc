// token.h

#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include <stdbool.h>
#include "pos.h"

#include "props.h"

#define TOK_RW(type, name) PROP_RW(Token, tok, type, name)
#define TOK_RO(type, name) PROP_RO(Token, tok, type, name)


enum {
    TIDENT,
    TKEYWORD,
    TNUMBER,
    TCHAR,
    TSTRING,
    TEOF,
    TINVALID,
    // Only in CPP
    MIN_CPP_TOKEN,
    TNEWLINE,
    TSPACE,
    TMACRO_PARAM,
};

enum {
    ENC_NONE,
    ENC_CHAR16,
    ENC_CHAR32,
    ENC_UTF8,
    ENC_WCHAR,
};

typedef struct Token Token;
typedef struct File File;
typedef struct Set Set;


Token *tok_alloc(int kind);

Token *tok_copy(Token *tok);
Token *tok_copy_as(Token *tok, int kind);

Token *tok_new_macro(int position, bool is_vararg);
Token *tok_new(int kind);

Token *tok_new_ident(char *p);
Token *tok_new_strtok(char *sval, int len, int enc);
Token *tok_new_keyword(int id);
Token *tok_new_number(char *sval);
Token *tok_new_invalid(char c);

Token *tok_new_char(int c, int enc);

TOK_RO(int, kind)
TOK_RW(File *, file)
TOK_RW(Pos, pos)
TOK_RW(bool, space)
TOK_RW(bool, bol)
TOK_RW(int, count)
TOK_RW(Set *, hideset)

TOK_RW(int, id)
TOK_RW(int, enc)
TOK_RW(const char *, sval)
TOK_RW(int, slen)
TOK_RW(int, c)
TOK_RW(int, position)
TOK_RW(bool, is_vararg)

bool tok_is_keyword(Token *tok, int c);
bool tok_is_ident(Token *tok, const char *s);

char *tok_position_string(Token *tok);

#endif // TOKEN_H_INCLUDED
