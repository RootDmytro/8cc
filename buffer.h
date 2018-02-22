// buffer.h

typedef struct Buffer Buffer;

Buffer *buf_alloc(void);
Buffer *buf_init(Buffer *b);
Buffer *buf_new(void);

char *buf_body(Buffer *b);
int buf_len(Buffer *b);
void buf_write(Buffer *b, char c);
void buf_append(Buffer *b, char *s, int len);
void buf_printf(Buffer *b, char *fmt, ...);

void buf_free(Buffer *b);

char *vformat(char *fmt, va_list ap);
char *format(char *fmt, ...);
char *quote_cstring(char *p);
char *quote_cstring_len(char *p, int len);
char *escape_char(char c);
