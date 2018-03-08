// Copyright 2014 Rui Ueyama. Released under the MIT license.

// This is an implementation of hash table.

#include <stdlib.h>
#include <string.h>
#include "8cc.h"

#define INIT_SIZE 16
#define TOMBSTONE ((void *)-1)


typedef struct Map {
    const struct Map *parent;
    char **key;
    void **val;
    int size;
    int nelem;
    int nused;
} Map;


static uint32_t hash(const char *p) {
    // FNV hash
    uint32_t r = 2166136261;
    for (; *p; p++) {
        r ^= *p;
        r *= 16777619;
    }
    return r;
}


Map *map_alloc(void) {
    Map *r = calloc(1, sizeof(Map));
    r->parent = NULL;
    r->key = NULL;
    r->val = NULL;
    return r;
}

Map *map_init(Map *m) {
    return map_init_parent(m, NULL);
}

Map *map_init_parent(Map *m, const Map *parent) {
    return map_init_size(m, parent, INIT_SIZE);
}

Map *map_init_size(Map *m, const Map *parent, int size) {
    m->parent = parent;
    m->key = calloc(size, sizeof(char *));
    m->val = calloc(size, sizeof(void *));
    m->size = size;
    m->nelem = 0;
    m->nused = 0;
    return m;
}

Map *map_new(void) {
    return map_init(map_alloc());
}

Map *map_new_parent(const Map *parent) {
    return map_init_parent(map_alloc(), parent);
}

static void maybe_rehash(Map *m) {
    if (!m->key) {
        m->key = calloc(INIT_SIZE, sizeof(char *));
        m->val = calloc(INIT_SIZE, sizeof(void *));
        m->size = INIT_SIZE;
        return;
    }
    if (m->nused < m->size * 0.7)
        return;
    int newsize = (m->nelem < m->size * 0.35) ? m->size : m->size * 2;
    char **k = calloc(newsize, sizeof(char *));
    void **v = calloc(newsize, sizeof(void *));
    int mask = newsize - 1;
    for (int i = 0; i < m->size; i++) {
        if (m->key[i] == NULL || m->key[i] == TOMBSTONE)
            continue;
        int j = hash(m->key[i]) & mask;
        for (;; j = (j + 1) & mask) {
            if (k[j] != NULL)
                continue;
            k[j] = m->key[i];
            v[j] = m->val[i];
            break;
        }
    }
    m->key = k;
    m->val = v;
    m->size = newsize;
    m->nused = m->nelem;
}

static void *map_get_nostack(const Map *m, const char *key) {
    if (!m->key)
        return NULL;
    int mask = m->size - 1;
    int i = hash(key) & mask;
    for (; m->key[i] != NULL; i = (i + 1) & mask)
        if (m->key[i] != TOMBSTONE && !strcmp(m->key[i], key))
            return m->val[i];
    return NULL;
}

void *map_get(const Map *m, const char *key) {
    void *v = map_get_nostack(m, key);
    if (v)
        return v;
    // Map is stackable. If no value is found,
    // continue searching from the parent.
    if (m->parent)
        return map_get(m->parent, key);
    return NULL;
}

void map_put(Map *m, const char *key, void *val) {
    maybe_rehash(m);
    int mask = m->size - 1;
    int i = hash(key) & mask;
    for (;; i = (i + 1) & mask) {
        char *k = m->key[i];
        if (k == NULL || k == TOMBSTONE) {
            m->key[i] = strdup(key);
            m->val[i] = val;
            m->nelem++;
            if (k == NULL)
                m->nused++;
            return;
        }
        if (!strcmp(k, key)) {
            m->val[i] = val;
            return;
        }
    }
}

void map_remove(Map *m, const char *key) {
    if (!m->key)
        return;
    int mask = m->size - 1;
    int i = hash(key) & mask;
    for (; m->key[i] != NULL; i = (i + 1) & mask) {
        if (m->key[i] == TOMBSTONE || strcmp(m->key[i], key))
            continue;
        m->key[i] = TOMBSTONE;
        m->val[i] = NULL;
        m->nelem--;
        return;
    }
}

size_t map_len(Map *m) {
    return m->nelem;
}

void map_free(Map *m) {
    m->parent = (void *)0xBadBadBadBadBadB;
    free(m->key);
    free(m->val);
}
