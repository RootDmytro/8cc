// pos.c

#include "pos.h"


Pos pos_make(int line, int column) {
    return (Pos){.line = line, .column = column};
}

int pos_line(Pos pos) {
    return pos.line;
}

int pos_column(Pos pos) {
    return pos.column;
}
