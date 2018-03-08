// file.h

#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

#include <time.h>

// 2. File must be renamed to Stream
typedef struct File File;
typedef struct String String;

struct _IO_FILE;
typedef struct _IO_FILE FILE;

File *file_alloc(void);
File *file_init(File *f, FILE *file, String *name);
File *file_init_string(File *f, const char *s);


String *file_name(File *f);
void file_set_name(File *f, String *name);

int file_line(File *f);
void file_set_line(File *f, int line);

int file_column(File *f);
void file_set_column(File *f, int column);

int file_ntok_increment(File *f);

time_t file_mtime(File *f);


int file_readc(File *f);
int file_getc(File *f);
void file_unreadc(File *f, int c);

void file_close(File *f);


void file_free(File *f);

#endif // FILE_H_INCLUDED
