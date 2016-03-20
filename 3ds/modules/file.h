#ifndef MICROPYTHON_FILE_H
#define MICROPYTHON_FILE_H

typedef struct _mp_obj_fdfile_t {
    mp_obj_base_t base;
    int fd;
} mp_obj_fdfile_t;

extern const mp_obj_type_t mp_type_fileio;
extern const mp_obj_type_t mp_type_textio;

#endif //MICROPYTHON_FILE_H
