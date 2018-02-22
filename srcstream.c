// srcstream.c

#include <stdio.h>
#include "file.h"
#include "vector.h"
#include "srcstream.h"

static Vector/*<File *> */ *files = EMPTY_VECTOR;
static Vector/*<Vector<File *> *> */ *stashed = EMPTY_VECTOR;

static int get() {
    File *f = vec_tail(files);
    int c = file_getc(f);
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
#warning 'files' relinquishing responsibility for value of 'f' without explicit handover (preceding reference sharing has occured)
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
    file_unreadc(f, c);
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
    return format("%s:%d:%d", file_name(f), file_line(f), file_column(f));
}

void stream_stash(File *file) {
    vec_push(stashed, files);
    files = vec_new1(file);
}

void stream_unstash() {
    files = vec_pop(stashed);
#warning memory leak: files
}
