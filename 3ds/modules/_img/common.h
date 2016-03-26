#ifndef _IMG_COMMON_H_
#define _IMG_COMMON_H_

typedef struct {
    unsigned char *buffer;
    size_t length;
    size_t cursor;
} mem_file;

#endif // _IMG_COMMON_H_