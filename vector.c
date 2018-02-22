// Copyright 2012 Rui Ueyama. Released under the MIT license.

/*
 * Vectors are containers of void pointers that can change in size.
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "vector.h"

#define MIN_SIZE 8


typedef struct Vector {
    void **body;
    int len;
    int nalloc;
} Vector;

///////////////////////////////////

static int max(int a, int b) {
    return a > b ? a : b;
}

static int round_to_pow2(int n) {
    if (n == 0)
        return 0;
    int r = 1;
    while (n > r)
        r *= 2;
    return r;
}

///////////////////////////////////

Vector *vec_alloc(void) {
    Vector *r = calloc(1, sizeof(Vector));
    r->body = NULL;
    return r;
}

Vector *vec_init(Vector *vec) {
    vec->body = NULL;
    vec->len = 0;
    vec->nalloc = 0;
    return vec;
}

Vector *vec_init_size(Vector *vec, int size) {
    size = round_to_pow2(size);

    if (size > 0) {
        vec->body = calloc(size, sizeof(void *));
    } else {
        vec->body = NULL;
    }

    vec->len = 0;
    vec->nalloc = size;
    return vec;
}

static void vec_extend(Vector *vec, int delta) {
    if (vec->len + delta <= vec->nalloc) {
        return;
    }

    int nelem = max(round_to_pow2(vec->len + delta), MIN_SIZE);
    void *newbody = calloc(nelem, sizeof(void *));

    if (vec->body) {
        memcpy(newbody, vec->body, sizeof(void *) * vec->len);
    }

    free(vec->body);
    vec->body = newbody;
    vec->nalloc = nelem;
}

Vector *vec_new(void) {
    return vec_init(vec_alloc());
}

Vector *vec_new1(void *elem) {
    Vector *vec = vec_init_size(vec_alloc(), 1);
    vec->body[vec->len++] = elem;
    return vec;
}

Vector *vec_copy(Vector *src) {
    Vector *vec = vec_init_size(vec_alloc(), src->len);
    memcpy(vec->body, src->body, sizeof(void *) * src->len);
    vec->len = src->len;
    return vec;
}

void vec_push(Vector *vec, void *elem) {
    vec_extend(vec, 1);
    vec->body[vec->len++] = elem;
}

void vec_append(Vector *a, Vector *b) {
    vec_extend(a, b->len);
    memcpy(a->body + a->len, b->body, sizeof(void *) * b->len);
    a->len += b->len;
}

void *vec_pop(Vector *vec) {
    assert(vec->len > 0);
    return vec->body[--vec->len];
}

void *vec_get(Vector *vec, int index) {
    assert(0 <= index && index < vec->len);
    return vec->body[index];
}

void vec_set(Vector *vec, int index, void *val) {
    assert(0 <= index && index < vec->len);
    vec->body[index] = val;
}

void *vec_head(Vector *vec) {
    assert(vec->len > 0);
    return vec->body[0];
}

void *vec_tail(Vector *vec) {
    assert(vec->len > 0);
    return vec->body[vec->len - 1];
}

Vector *vec_reverse(Vector *vec) {
    Vector *r = vec_init_size(vec_alloc(), vec->len);
    for (int i = 0; i < vec->len; i++)
        r->body[i] = vec->body[vec->len - i - 1];
    r->len = vec->len;
    return r;
}

void *vec_body(Vector *vec) {
    return vec->body;
}

int vec_len(Vector *vec) {
    return vec->len;
}

void vec_each(Vector *vec, fptr_t *fptr) {
    for (int i = 0; i < vec->len; i++) {
        fptr(vec->body[i]);
    }
}

void vec_free(Vector *vec) {
    free(vec->body);
    free(vec);
}
