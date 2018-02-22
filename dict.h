// dict.h

typedef struct Dict Dict;
typedef struct Vector Vector;

Dict *dict_alloc(void);
Dict *dict_init(Dict *r);
Dict *dict_new(void);

void *dict_get(Dict *dict, char *key);
void dict_put(Dict *dict, char *key, void *val);
Vector *dict_keys(Dict *dict);

void dict_free(Dict *r);
