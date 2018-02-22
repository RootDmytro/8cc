// vector.h

#warning this shold be removed later
typedef struct _Vector {
    void **r0;
    int r1;
    int r2;
} _Vector;
#define EMPTY_VECTOR ((Vector *)&((_Vector){}))

typedef struct Vector Vector;
typedef void (fptr_t)(void *el);

Vector *vec_alloc(void);
Vector *vec_init(Vector *vec);
Vector *vec_init_size(Vector *vec, int size);

Vector *vec_new(void);
Vector *vec_new1(void *elem);
Vector *vec_copy(Vector *src);

void vec_push(Vector *vec, void *elem);
void vec_append(Vector *a, Vector *b);
void *vec_pop(Vector *vec);
void *vec_get(Vector *vec, int index);
void vec_set(Vector *vec, int index, void *val);
void *vec_head(Vector *vec);
void *vec_tail(Vector *vec);
Vector *vec_reverse(Vector *vec);
void *vec_body(Vector *vec);
int vec_len(Vector *vec);
void vec_each(Vector *vec, fptr_t *fptr);

void vec_free(Vector *vec);
