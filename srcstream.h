// srcstream.h

#ifndef SRCSTREAM_H_INCLUDED
#define SRCSTREAM_H_INCLUDED

struct File;
typedef struct File File;

// 1. following must be in a separate file
int readc(void);
void unreadc(int c);
File *current_file(void);
void stream_push(File *file);
int stream_depth(void);
char *input_position(void);
void stream_stash(File *file);
void stream_unstash(void);

#endif // SRCSTREAM_H_INCLUDED
