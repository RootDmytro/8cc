// srcloc.h

#ifndef SRCLOC_H_INCLUDED
#define SRCLOC_H_INCLUDED

#include "object.h"

typedef struct SourceLoc SourceLoc;
typedef struct String String;

SourceLoc *srcloc_alloc(void);
SourceLoc *srcloc_init(SourceLoc *loc, String *file, int line);

SourceLoc *srcloc_new(String *file, int line);

String *srcloc_filename(SourceLoc *loc);
int srcloc_line(SourceLoc *loc);

int srcloc_cmp(SourceLoc *loc, SourceLoc *other);

SourceLoc *srcloc_retain(SourceLoc *loc);
void srcloc_release(SourceLoc *loc);
SourceLoc *srcloc_assign(SourceLoc **pointer, SourceLoc *newValue);

#endif // SRCLOC_H_INCLUDED
