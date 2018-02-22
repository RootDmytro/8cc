// object.c

#include <stdlib.h>
#include <assert.h>
#include "object.h"


typedef struct Object {
    long refcount;
    // yes, it should be a pointer to a vtbl or class descriptor of some sort, but it not necessary right now
    deinit_ptr_t *deinit;
} Object;


Object *obj_new(unsigned long size, deinit_ptr_t *deinit) {
    Object *r = calloc(1, sizeof(Object) + size);
    r->deinit = deinit;
    ++r;
    return r;
}

Object *obj_retain(Object *object) {
    ++object[-1].refcount;
    return object;
}

void obj_release(Object *object) {
    if (!--object[-1].refcount) {
        obj_free(object);
    }
}

void obj_free(Object *object) {
    deinit_ptr_t *deinit = object[-1].deinit;
    if (deinit != NULL) {
        deinit(object);
    }
    free(object);
}
