// Copyright 2012 Rui Ueyama. Released under the MIT license.

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

#define INIT_SIZE 8


typedef struct Buffer {
    char *body;
    int nalloc;
    int len;
} Buffer;

Buffer *buf_alloc(void) {
    Buffer *r = calloc(1, sizeof(Buffer));
    r->body = NULL;
    return r;
}

Buffer *buf_init(Buffer *b) {
    b->nalloc = INIT_SIZE;
    b->body = malloc(b->nalloc);
    b->len = 0;
    return b;
}

Buffer *buf_new(void) {
    return buf_init(buf_alloc());
}

static void buf_realloc_body(Buffer *b) {
    b->nalloc = b->nalloc * 2;
    char *body = malloc(b->nalloc);
    memcpy(body, b->body, b->len);
    free(b->body);
    b->body = body;
}

char *buf_body(Buffer *b) {
    return b->body;
}

int buf_len(Buffer *b) {
    return b->len;
}

void buf_write(Buffer *b, char c) {
    if (b->nalloc == (b->len + 1))
        buf_realloc_body(b);
    b->body[b->len++] = c;
}

void buf_append(Buffer *b, char *s, int len) {
    for (int i = 0; i < len; i++)
        buf_write(b, s[i]);
}

void buf_printf(Buffer *b, char *fmt, ...) {
    va_list args;
    for (;;) {
        int avail = b->nalloc - b->len;
        va_start(args, fmt);
        int written = vsnprintf(b->body + b->len, avail, fmt, args);
        va_end(args);
        if (avail <= written) {
            buf_realloc_body(b);
            continue;
        }
        b->len += written;
        return;
    }
}

void buf_free(Buffer *b) {
    free(b->body);
    free(b);
}



char *vformat(const char *fmt, va_list ap) {
    Buffer *b = buf_init(buf_alloc());
    va_list aq;
    int written;

    for (;;) {
        int avail = b->nalloc - b->len;
        va_copy(aq, ap);
        written = vsnprintf(b->body + b->len, avail, fmt, aq);
        va_end(aq);

        if (avail > written) {
            break;
        }

        buf_realloc_body(b);
    }

    b->len += written;
    return buf_body(b);
#warning memory leak: b
#warning memory leak: b->body escaping
}

char *format(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char *r = vformat(fmt, ap);
    va_end(ap);
    return r;
}

static char *escape(char c) {
    switch (c) {
    case '"': return "\\\"";
    case '\\': return "\\\\";
    case '\b': return "\\b";
    case '\f': return "\\f";
    case '\n': return "\\n";
    case '\r': return "\\r";
    case '\t': return "\\t";
    }
    return NULL;
}

static void print(Buffer *b, char c) {
    char *q = escape(c);
    if (q) {
        buf_printf(b, "%s", q);
    } else if (isprint(c)) {
        buf_printf(b, "%c", c);
    } else {
        buf_printf(b, "\\x%02x", c);
    }
}

char *quote_cstring(const char *p) {
    Buffer *b = buf_init(buf_alloc());
    while (*p)
        print(b, *p++);
    return buf_body(b);
#warning memory leak: b
#warning memory leak: b->body escaping
}

char *quote_cstring_len(const char *p, int len) {
    Buffer *b = buf_init(buf_alloc());
    for (int i = 0; i < len; i++)
        print(b, p[i]);
    return buf_body(b);
#warning memory leak: b
#warning memory leak: b->body escaping
}

char *escape_char(char c) {
    if (c == '\\') return "\\\\";
    if (c == '\'') return "\\'";
    return format("%c", c);
}
