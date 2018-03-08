// srcloc.c

#include <stdlib.h>
#include "str.h"
#include "srcloc.h"


typedef struct SourceLoc {
    String *filepath;
    int line;
} SourceLoc;


static void srcloc_deinit(SourceLoc *loc) {
    str_assign(&loc->filepath, Nil);
}

SourceLoc *srcloc_alloc(void) {
    SourceLoc *r = (SourceLoc *)obj_new(sizeof(SourceLoc), (void(*)(Object *))srcloc_deinit);
    return r;
}

SourceLoc *srcloc_init(SourceLoc *loc, String *filepath, int line) {
    str_assign(&loc->filepath, filepath);
    loc->line = 0;
    return loc;
}

SourceLoc *srcloc_new(String *filepath, int line) {
    return srcloc_init(srcloc_alloc(), filepath, line);
}

String *srcloc_filename(SourceLoc *loc) {
    return loc->filepath;
}

int srcloc_line(SourceLoc *loc) {
    return loc->line;
}

int srcloc_cmp(SourceLoc *loc, SourceLoc *other) {
    int relation = str_cmp(srcloc_filename(loc), srcloc_filename(other));

    if (relation == 0 && loc->line != other->line) {
        relation = loc->line > other->line ? 1 : -1;
    }
    return relation;
}

SourceLoc *srcloc_retain(SourceLoc *object) {
    return (SourceLoc *)obj_retain((Object *)object);
}

void srcloc_release(SourceLoc *object) {
    obj_release((Object *)object);
}

SourceLoc *srcloc_assign(SourceLoc **pointer, SourceLoc *newValue) {
    return (SourceLoc *)obj_assign((Object **)pointer, (Object *)newValue);
}
