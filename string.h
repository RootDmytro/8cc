// string.h

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include "object.h"

typedef struct String String;

String *str_alloc(unsigned long length);
String *str_init(String *str, const char *string);
String *str_init_length(String *str, const char *string, unsigned long length);

String *str_new(const char *str);
String *str_new_length(const char *string, unsigned long length);
String *str_copy(String *str);

const char *str_get(String *vec);
unsigned long str_length(String *vec);

String *str_retain(String *object);
void str_release(String *object);

#endif // STRING_H_INCLUDED
