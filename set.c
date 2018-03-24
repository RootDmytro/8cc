// Copyright 2014 Rui Ueyama. Released under the MIT license.

// Sets are containers that store unique strings.
//
// The data structure is functional. Because no destructive
// operation is defined, it's guranteed that a set will never
// change once it's created.
//
// A null pointer represents an empty set.
//
// Set is designed with simplicity in mind.
// It should be very fast for small number of items.
// However, if you plan to add a lot of items to a set,
// you should consider using Map as a set.

#include <stdlib.h>
#include <string.h>
#include "set.h"


typedef struct Set {
    char *value;
    Set *next;
} Set;


Set *set_alloc(void) {
    Set *r = calloc(1, sizeof(Set));
    r->next = NULL;
    return r;
}

Set *set_init(Set *s, Set *next, char *value) {
    s->next = next;
    s->value = value;
    return s;
}

Set *set_add(Set *s, char *value) {
    Set *r = set_alloc();
    r = set_init(r, s, value);
    return r;
}

bool set_has(Set *s, const char *value) {
    for (; s; s = s->next)
        if (strcmp(s->value, value) == 0)
            return 1;
    return false;
}

Set *set_union(Set *a, Set *b) {
    Set *r = b;
    for (; a; a = a->next)
        if (!set_has(b, a->value))
            r = set_add(r, a->value);
    return r;
}

Set *set_intersection(Set *a, Set *b) {
    Set *r = NULL;
    for (; a; a = a->next)
        if (set_has(b, a->value))
            r = set_add(r, a->value);
    return r;
}

void set_free(Set *s) {
    for (Set *next; s; s = next) {
        next = s->next;
        free(s);
    }
}
