// object.c

#include <stdlib.h>
#include <assert.h>
#include "object.h"


typedef struct Object {
    long refcount;
    // yes, it should be a pointer to a vtbl or class descriptor of some sort, but it is not necessary right now
    deinit_ptr_t *deinit;
} Object;


Object *obj_new(unsigned long size, deinit_ptr_t *deinit) {
    Object *r = calloc(1, sizeof(Object) + size);
    r->deinit = deinit;
    ++r;
    return r;
}

Object *obj_retain(Object *object) {
    if (object != Nil) {
        ++object[-1].refcount;
    }
    return object;
}

void obj_release(Object *object) {
    if (object != Nil && !--object[-1].refcount) {
        _obj_free(object);
    }
}

void _obj_free(Object *object) {
    if (object != Nil) {
        Object *r = object - 1;
        deinit_ptr_t *deinit = r->deinit;

        if (deinit != NULL) {
            deinit(object);
        }

        free(r);
    }
}

Object *obj_assign(Object **pointer, Object *newValue) {
    Object *oldObject = *pointer;
    *pointer = obj_retain(newValue);
    obj_release(oldObject);
    return newValue;
}
