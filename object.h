// object.h
//
// Base class for objects that wish to use basic MRC

typedef struct Object Object;

typedef void (deinit_ptr_t)(Object *object);

/** creates an object of given size with additional hidden members for memory management
    size - memory space required by inheriting class
    deinit - (optional) a function that is called before freeing the object
    returns a new object that has ref. count = 0
 */
Object *obj_new(unsigned long size, deinit_ptr_t *deinit);

Object *obj_retain(Object *object);
void obj_release(Object *object);

/// don't use this method unless you are sure that you know what you are doing
void obj_free(Object *object);
