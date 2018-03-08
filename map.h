// map.h

#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#warning this shold be removed later
typedef struct _Map {
    struct Map *r0;
    char **r1;
    void **r2;
    int r3;
    int r4;
    int r5;
} _Map;
#define EMPTY_MAP ((Map *)&((_Map){}))

typedef struct Map Map;

Map *map_alloc(void);
Map *map_init(Map *m);
Map *map_init_parent(Map *m, const Map *parent);
Map *map_init_size(Map *m, const Map *parent, int size);

Map *map_new(void);
Map *map_new_parent(const Map *parent);

void *map_get(const Map *m, const char *key);
void map_put(Map *m, const char *key, void *val);
void map_remove(Map *m, const char *key);
size_t map_len(Map *m);

void map_free(Map *m);

#endif // MAP_H_INCLUDED
