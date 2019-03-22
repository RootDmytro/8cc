// encoding.h

#ifndef ENCODING_H_INCLUDED
#define ENCODING_H_INCLUDED

Buffer *to_utf16(char *p, int len);
Buffer *to_utf32(char *p, int len);
void write_utf8(Buffer *b, uint32_t rune);

#endif // ENCODING_H_INCLUDED
