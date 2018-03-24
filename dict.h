// dict.h

#ifndef DICT_H_INCLUDED
#define DICT_H_INCLUDED

typedef struct Dict Dict;
typedef struct Vector Vector;

Dict *dict_alloc(void);
Dict *dict_init(Dict *r);
Dict *dict_new(void);

void *dict_get(Dict *dict, const char *key);
void dict_put(Dict *dict, const char *key, void *val);
Vector *dict_keys(Dict *dict);

void dict_free(Dict *r);

#endif // DICT_H_INCLUDED
