// file.h

// 2. File must be renamed to Stream
typedef struct File File;

struct _IO_FILE;
typedef struct _IO_FILE FILE;

File *file_alloc(void);
File *file_init(File *f, FILE *file, const char *name);
File *file_init_string(File *f, char *s);

const char *file_name(File *f);
void file_set_name(File *f, const char *name);

int file_line(File *f);
void file_set_line(File *f, int line);

int file_column(File *f);
void file_set_column(File *f, int column);

int file_ntok_increment(File *f);

time_t file_mtime(File *f);

void file_free(File *f);

// 1. following must be in a separate file
int readc(void);
void unreadc(int c);
File *current_file(void);
void stream_push(File *file);
int stream_depth(void);
char *input_position(void);
void stream_stash(File *f);
void stream_unstash(void);
