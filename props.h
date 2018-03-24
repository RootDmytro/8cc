
#ifndef PROPS_H_INCLUDED
#define PROPS_H_INCLUDED

#define PROP_RO(class, class_short, type, name) \
    type class_short##_##name(class *class_short);

#define PROP_RW(class, class_short, type, name) \
    PROP_RO(class, class_short, type, name) \
    void class_short##_set_##name(class *class_short, type name);

#define PROP_DEF_RO(class, class_short, type, name) \
    type class_short##_##name(class *class_short) { return class_short->name; }

#define PROP_DEF_RW(class, class_short, type, name) \
    PROP_DEF_RO(class, class_short, type, name) \
    void class_short##_set_##name(class *class_short, type name) { class_short->name = name; }

#define PROP_CDEF_RO(class, class_short, type, name, acc) \
    type class_short##_##name(class *class_short) { return acc; }

#define PROP_CDEF_RW(class, class_short, type, name, acc) \
    PROP_CDEF_RO(class, class_short, type, name, acc) \
    void class_short##_set_##name(class *class_short, type name) { acc = name; }

#endif // PROPS_H_INCLUDED
