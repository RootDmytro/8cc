// Copyright 2014 Rui Ueyama. Released under the MIT license.

/*
 * This file provides character input stream for C source code.
 * An input stream is either backed by stdio's FILE * or
 * backed by a string.
 * The following input processing is done at this stage.
 *
 * - C11 5.1.1.2p1: "\r\n" or "\r" are canonicalized to "\n".
 * - C11 5.1.1.2p2: A sequence of backslash and newline is removed.
 * - EOF not immediately following a newline is converted to
 *   a sequence of newline and EOF. (The C spec requires source
 *   files end in a newline character (5.1.1.2p2). Thus, if all
 *   source files are comforming, this step wouldn't be needed.)
 *
 * Trigraphs are not supported by design.
 */

#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "file.h"


typedef struct File_vtbl {
    void (*file_close)(File *f);
    int (*file_readc)(File *f);
} File_vtbl;


typedef struct File {
    File_vtbl *vptr;
    union {
        FILE *file;  // stream backed by FILE *
        char *p;     // stream backed by string
    };
    const char *name;
    int line;
    int column;
    int ntok;     // token counter
    int last;     // the last character read from file
    int buf[3];   // push-back buffer for unread operations
    int buflen;   // push-back buffer size
    time_t mtime; // last modified time. 0 if string-backed file
} File;


void file_close_file(File *f);
void file_close_string(File *f);

int file_readc_file(File *f);
int file_readc_string(File *f);


File_vtbl File_vtable = {file_close_file, file_readc_file};
File_vtbl FileString_vtbl = {file_close_string, file_readc_string};


File *file_alloc(void) {
    File *r = calloc(1, sizeof(File));
    return r;
}


File *file_init(File *f, FILE *file, const char *name) {
    assert(file != NULL);
    assert(name != NULL);
    f->vptr = &File_vtable;
    f->file = file;
    f->name = strdup(name);
    f->line = 1;
    f->column = 1;

    struct stat st;
    if (fstat(fileno(file), &st) == -1) {
        error("fstat failed: %s", strerror(errno));
    }
    f->mtime = st.st_mtime;
    return f;
}

File *file_init_string(File *f, char *s) {
    assert(s != NULL);
    f->vptr = &FileString_vtbl;
    f->p = s;
    f->line = 1;
    f->column = 1;
    return f;
}

const char *file_name(File *f) {
    return f->name;
}

void file_set_name(File *f, const char *name) {
#warning this must be solved by introduction of String objects with reference counting
    //free((void *)f->name);
    f->name = /*strdup*/(name);
}

int file_line(File *f) {
    return f->line;
}

void file_set_line(File *f, int line) {
    f->line = line;
}

int file_column(File *f) {
    return f->column;
}

void file_set_column(File *f, int column) {
    f->column = column;
}

int file_ntok_increment(File *f) {
    return f->ntok++;
}

time_t file_mtime(File *f) {
    return f->mtime;
}

int file_readc(File *f) {
    return f->vptr->file_readc(f);
}

int file_readc_file(File *f) {
    int c = getc(f->file);

    if (c == EOF) {
        c = (f->last == '\n' || f->last == EOF) ? EOF : '\n';
    } else if (c == '\r') {
        int c2 = getc(f->file);

        if (c2 != '\n') {
            ungetc(c2, f->file);
        }

        c = '\n';
    }

    f->last = c;
    return c;
}

int file_readc_string(File *f) {
    int c;

    if (*f->p == '\0') {
        c = (f->last == '\n' || f->last == EOF) ? EOF : '\n';
    } else if (*f->p == '\r') {
        f->p++;

        if (*f->p == '\n') {
            f->p++;
        }

        c = '\n';
    } else {
        c = *f->p++;
    }

    f->last = c;
    return c;
}

int file_getc(File *f) {
    int c;

    if (f->buflen > 0) {
        c = f->buf[--f->buflen];
    } else {
        c = file_readc(f);
    }

    if (c == '\n') {
        f->line++;
        f->column = 1;
    } else if (c != EOF) {
        f->column++;
    }
    return c;
}

void file_unreadc(File *f, int c) {
    if (c == EOF) {
        return;
    }

    assert(f->buflen < sizeof(f->buf) / sizeof(f->buf[0]));
    f->buf[f->buflen++] = c;

    if (c == '\n') {
        f->column = 1;
        f->line--;
    } else {
        f->column--;
    }
}

void file_close(File *f) {
    f->vptr->file_close(f);
}

void file_close_file(File *f) {
    fclose(f->file);
    f->file = NULL;
}

void file_close_string(File *f) {
    free(f->p);
    f->p = NULL;
}

void file_free(File *f) {
    file_close(f);

    free((void *)f->name);
    f->name = NULL;

    free(f);
}

