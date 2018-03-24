// token.c

#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "srcstream.h"
#include "lex.h"
#include "file.h"

#define TOK_DEF_RO(type, name) PROP_DEF_RO(Token, tok, type, name)
#define TOK_DEF_RW(type, name) PROP_DEF_RW(Token, tok, type, name)

#define TOK_CDEF_RO(type, name) PROP_CDEF_RO(Token, tok, type, name, tok->super.name)
#define TOK_CDEF_RW(type, name) PROP_CDEF_RW(Token, tok, type, name, tok->super.name)

typedef struct TokenSuperclass {
    int kind;
    File *file;
    Pos pos;
    bool space;   // true if the token has a leading space
    bool bol;     // true if the token is at the beginning of a line
    int count;    // token number in a file, counting from 0.
    Set *hideset; // used by the preprocessor for macro expansion
} TokenSuperclass;

typedef struct Token {
    TokenSuperclass super;

    union {
        // TKEYWORD
        int id;
        // TIDENT, TSTRING, TNUMBER, TINVALID, TCHAR
        struct {
            int enc;
            union {
                struct {
                    const char *sval;
                    int slen;
                };
                int c;
            };
        };
        // TMACRO_PARAM
        struct {
            bool is_vararg;
            int position;
        };
    };
} Token;

Token *tok_alloc(int kind) {
    Token *r = calloc(1, sizeof(Token));
    r->super.kind = kind;
    return r;
}

Token *tok_copy(Token *tok) {
    Token *r = tok_alloc(tok_kind(tok));
    *r = *tok;
    return r;
}

Token *tok_copy_as(Token *tok, int kind) {
    Token *r = tok_alloc(kind);

    if (kind == tok_kind(tok)) {
        *r = *tok;
    } else {
        *(TokenSuperclass *)r = *(TokenSuperclass *)tok;
    }
    r->super.kind = kind;

    return r;
}

Token *tok_new_macro(int position, bool is_vararg) {
    Token *r = tok_alloc(TMACRO_PARAM);
    r->is_vararg = is_vararg;
    r->super.hideset = NULL;
    r->position = position;
    r->super.space = false;
    r->super.bol = false;
    return r;
}

Token *tok_new(int kind) {
    Token *r = tok_alloc(kind);
    r->super.hideset = NULL;

    File *f = current_file();
    r->super.file = f;
    r->super.pos = lex_current_pos();
    r->super.count = file_ntok_increment(f);
    return r;
}

Token *tok_new_ident(char *p) {
    Token *tok = tok_new(TIDENT);
    tok->sval = p;
    return tok;
}

Token *tok_new_strtok(char *sval, int len, int enc) {
    Token *tok = tok_new(TSTRING);
    tok->sval = sval;
    tok->slen = len;
    tok->enc = enc;
    return tok;
}

Token *tok_new_keyword(int id) {
    Token *tok = tok_new(TKEYWORD);
    tok->id = id;
    return tok;
}

Token *tok_new_number(char *sval) {
    Token *tok = tok_new(TNUMBER);
    tok->sval = sval;
    return tok;
}

Token *tok_new_invalid(char c) {
    Token *tok = tok_new(TINVALID);
    tok->c = c;
    return tok;
}

Token *tok_new_char(int c, int enc) {
    Token *tok = tok_new(TCHAR);
    tok->c = c;
    tok->enc = enc;
    return tok;
}


TOK_CDEF_RO(int, kind)
TOK_CDEF_RW(File *, file)
TOK_CDEF_RW(Pos, pos)
TOK_CDEF_RW(bool, space)
TOK_CDEF_RW(bool, bol)
TOK_CDEF_RW(int, count)
TOK_CDEF_RW(Set *, hideset)

TOK_DEF_RW(int, id)
TOK_DEF_RW(int, enc)
TOK_DEF_RW(const char *, sval)
TOK_DEF_RW(int, slen)
TOK_DEF_RW(int, c)
TOK_DEF_RW(int, position)
TOK_DEF_RW(bool, is_vararg)


bool tok_is_keyword(Token *tok, int c) {
    return tok_kind(tok) == TKEYWORD && tok_id(tok) == c;
}

bool tok_is_ident(Token *tok, const char *s) {
    return tok_kind(tok) == TIDENT && !strcmp(tok_sval(tok), s);
}


/*
int tok_kind(Token *tok) {
    return tok->super.kind;
}

File *tok_file(Token *tok) {
    return tok->super.file;
}
void tok_set_file(Token *tok, File *file) {
    tok->super.file = file;
}

bool tok_space(Token *tok) {
    return tok->super.space;
}
void tok_set_space(Token *tok, bool space) {
    tok->super.space = space;
}

bool tok_bol(Token *tok) {
    return tok->super.bol;
}
void tok_set_bol(Token *tok, bool bol) {
    tok->super.bol = bol;
}

Set *tok_hideset(Token *tok) {
    return tok->super.hideset;
}
void tok_set_hideset(Token *tok, Set *hideset) {
    tok->super.hideset = hideset;
}


const char *tok_sval(Token *tok) {
    return tok->sval;
}
void tok_set_sval(Token *tok, const char *sval) {
    tok->sval = sval;
}

int tok_slen(Token *tok) {
    return tok->slen;
}
void tok_set_slen(Token *tok, int slen) {
    tok->slen = slen;
}

int tok_enc(Token *tok) {
    return tok->enc;
}
void tok_set_enc(Token *tok, int enc) {
    tok->enc = enc;
}

int tok_position(Token *tok) {
    return tok->position;
}
void tok_set_position(Token *tok, int position) {
    tok->position = position;
}

bool tok_is_vararg(Token *tok) {
    return tok->is_vararg;
}
void tok_set_is_vararg(Token *tok, bool is_vararg) {
    tok->is_vararg = is_vararg;
}
*/
