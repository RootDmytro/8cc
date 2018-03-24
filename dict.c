// Copyright 2012 Rui Ueyama. Released under the MIT license.

#include <stdlib.h>
#include "dict.h"
#include "map.h"
#include "vector.h"


typedef struct Dict {
    Map *map;
    Vector *key;
} Dict;


Dict *dict_alloc(void) {
    Dict *r = calloc(1, sizeof(Dict));
    r->map = NULL;
    r->key = NULL;
    return r;
}

Dict *dict_init(Dict *dict) {
    dict->map = map_new();
    dict->key = vec_new();
    return dict;
}

Dict *dict_new(void) {
    return dict_init(dict_alloc());
}

void *dict_get(Dict *dict, const char *key) {
    return map_get(dict->map, key);
}

void dict_put(Dict *dict, const char *key, void *val) {
    map_put(dict->map, key, val);
    vec_push(dict->key, key);
}

Vector *dict_keys(Dict *dict) {
    return dict->key;
}

void dict_free(Dict *dict) {
    map_free(dict->map);
    vec_free(dict->key);
    free(dict);
}
