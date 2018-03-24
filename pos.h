// pos.h

#ifndef POS_H_INCLUDED
#define POS_H_INCLUDED

typedef struct Pos {
    int line;
    int column;
} Pos;

Pos pos_make(int line, int column);

int pos_line(Pos pos);
int pos_column(Pos pos);

#endif // POS_H_INCLUDED
