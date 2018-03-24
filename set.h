// set.h

#include <stdbool.h>

typedef struct Set Set;

Set *set_alloc(void);
Set *set_init(Set *s, Set *next, char *value);

Set *set_add(Set *s, char *value);
bool set_has(Set *s, const char *value);
Set *set_union(Set *a, Set *b);
Set *set_intersection(Set *a, Set *b);

void set_free(Set *s);
