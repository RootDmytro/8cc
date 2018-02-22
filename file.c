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


typedef struct File {
    FILE *file;  // stream backed by FILE *
    char *p;     // stream backed by string
    const char *name;
    int line;
    int column;
    int ntok;     // token counter
    int last;     // the last character read from file
    int buf[3];   // push-back buffer for unread operations
    int buflen;   // push-back buffer size
    time_t mtime; // last modified time. 0 if string-backed file
} File;


File *file_alloc(void) {
    File *r = calloc(1, sizeof(File));
    r->file = NULL;
    r->p = NULL;
    r->name = NULL;
    return r;
}


File *file_init(File *f, FILE *file, const char *name) {
    f->file = file;
    f->p = NULL;
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
    f->file = NULL;
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

static void file_close(File *f) {
#warning virtual method must be used here
    if (f->file) {
        fclose(f->file);
        f->file = NULL;
    } else {
        free(f->p);
        f->file = NULL;
    }
}

static int file_readc(File *f) {
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

static int file_readc_string(File *f) {
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

void file_free(File *f) {
    file_close(f);
    f->file = NULL;

    free((void *)f->name);
    f->name = NULL;

    free(f);
}

////////////////////////////////////
// following must be in a separate file

#include "vector.h"

static Vector *files = EMPTY_VECTOR;
static Vector *stashed = EMPTY_VECTOR;

static int get() {
    File *f = vec_tail(files);
    int c;

    if (f->buflen > 0) {
        c = f->buf[--f->buflen];
    } else if (f->file) {
#warning virtual method must be used here
        c = file_readc(f);
    } else {
        c = file_readc_string(f);
    }

    if (c == '\n') {
        f->line++;
        f->column = 1;
    } else if (c != EOF) {
        f->column++;
    }
    return c;
}

int readc() {
    int c;

    for (;;) {
        c = get();

        if (c == EOF) {
            if (vec_len(files) == 1) {
                break;
            }

            File *f = vec_pop(files);
            file_close(f);
#warning 'files' relinquishing responsibility for 'f' without explicit handover (preceding reference sharing has occured)
            continue;
        }

        if (c != '\\') {
            break;
        }

        int c2 = get();

        if (c2 != '\n') {
            unreadc(c2);
            break;
        }
    }

    return c;
}

void unreadc(int c) {
    if (c == EOF) {
        return;
    }

    File *f = vec_tail(files);
    assert(f->buflen < sizeof(f->buf) / sizeof(f->buf[0]));
    f->buf[f->buflen++] = c;

    if (c == '\n') {
        f->column = 1;
        f->line--;
    } else {
        f->column--;
    }
}

File *current_file() {
    return vec_tail(files);
}

void stream_push(File *f) {
    vec_push(files, f);
}

int stream_depth() {
    return vec_len(files);
}

char *input_position() {
    if (vec_len(files) == 0) {
        return "(unknown)";
    }

    File *f = vec_tail(files);
    return format("%s:%d:%d", f->name, f->line, f->column);
}

void stream_stash(File *f) {
    vec_push(stashed, files);
    files = vec_new1(f);
}

void stream_unstash() {
    files = vec_pop(stashed);
#warning memory leak: files
}
