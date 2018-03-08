// string.c

#include "str.h"
#include <string.h>


typedef struct String {
    char string[1];
} String;


String *str_alloc(unsigned long length) {
    String *r = (String *)obj_new(length + 1, NULL);
    return r;
}

String *str_init(String *str, const char *string) {
    return str_init_length(str, string, strlen(string));
}

String *str_init_length(String *str, const char *string, unsigned long length) {
    memcpy(str->string, string, length);
    str->string[length] = 0;
    return str;
}

String *str_new(const char *string) {
    return str_new_length(string, strlen(string));
}

String *str_new_length(const char *string, unsigned long length) {
    String *r = str_init_length(str_alloc(length), string, length);
    return r;
}

String *str_copy(const String *str) {
    return str_new(str_get(str));
}

const char *str_get(const String *str) {
    return str->string;
}

unsigned long str_length(const String *str) {
    return strlen(str_get(str));
}

int str_cmp(const String *str, const String *other) {
    return strcmp(str_get(str), str_get(other));
}

String *str_retain(String *object) {
    return (String *)obj_retain((Object *)object);
}

void str_release(String *object) {
    obj_release((Object *)object);
}

String *str_assign(String **pointer, String *newValue) {
    return (String *)obj_assign((Object **)pointer, (Object *)newValue);
}
