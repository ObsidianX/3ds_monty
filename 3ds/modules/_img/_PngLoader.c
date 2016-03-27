#include <malloc.h>
#include <string.h>
#include <unistd.h>

#include <3ds.h>
#include <png.h>

#include "py/runtime.h"
#include "py/objstr.h"
#include "py/stream.h"

#include "../file.h"
#include "common.h"

#define SELF(src) mod__img__PngLoader_t *self = src

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod__img__PngLoader_##__n##_obj, mod__img__PngLoader_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod__img__PngLoader_##__n##_obj}

#define BG_THREAD_STACK_SIZE (2 * 1024)

const mp_obj_type_t mod__img__PngLoader_type;

STATIC mp_obj_t mod__img__PngLoader_load_all(mp_obj_t self_in);

typedef struct {
    mp_obj_base_t base;

    FILE *file;
    mem_file memfile;

    int width;
    int height;
    int channels;

    png_structp png;
    png_infop png_info;

    png_bytep img;
    png_bytepp rows;

    size_t row_len;
    size_t img_len;

    Thread bg_thread;
    bool bg_complete;
    bool bg_finished;
} mod__img__PngLoader_t;

STATIC void _mod_img_mem_file_read(png_structp png, png_bytep output, png_size_t size) {
    mem_file *file = png_get_io_ptr(png);

    if (file->cursor >= file->length) {
        return;
    }
    png_size_t _size = size;
    if (file->cursor + _size >= file->length) {
        _size = file->length - file->cursor;
    }
    memcpy(output, file->buffer + file->cursor, _size);
    file->cursor += _size;
}

STATIC void _mod__img__PngLoader_bg_thread(void *self_in) {
    SELF(self_in);

    mod__img__PngLoader_load_all(self_in);

    self->bg_complete = true;

    threadExit(0);
}

STATIC void _mod__img__PngLoader_setup(mp_obj_t self_in, bool from_memory) {
    SELF(self_in);

    unsigned char signature[8];
    if (from_memory) {
        _mod_img_mem_file_read(self->png, signature, sizeof(signature));
    } else {
        fread(signature, sizeof(signature), 1, self->file);
    }

    if (png_sig_cmp(signature, 0, 8)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "PNG data not found"));
    }

    self->png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!self->png) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_RuntimeError, "couldn't create png_struct"));
    }

    self->png_info = png_create_info_struct(self->png);
    if (!self->png_info) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_RuntimeError, "couldn't create png_info"));
    }

    if (from_memory) {
        png_set_read_fn(self->png, &self->memfile, &_mod_img_mem_file_read);
    } else {
        png_init_io(self->png, self->file);
    }

    png_set_sig_bytes(self->png, 8);

    png_read_info(self->png, self->png_info);

    self->width = png_get_image_width(self->png, self->png_info);
    self->height = png_get_image_height(self->png, self->png_info);
    self->channels = png_get_channels(self->png, self->png_info);

    self->row_len = png_get_rowbytes(self->png, self->png_info);
    self->img_len = sizeof(png_byte) * self->height * self->row_len;
    self->img = malloc(self->img_len);

    self->rows = malloc(sizeof(png_bytep) * self->height);
    for (int y = 0; y < self->height; y++) {
        self->rows[y] = &self->img[y * self->row_len];
    }
}

enum {
    NEW_ARG_TEXT = 0,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod__img__PngLoader_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 1, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_data, MP_ARG_OBJ | MP_ARG_REQUIRED},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod__img__PngLoader_t *obj = m_new_obj(mod__img__PngLoader_t);
    obj->base.type = (mp_obj_t) &mod__img__PngLoader_type;

    mp_obj_t _data = args[NEW_ARG_TEXT].u_obj;
    obj->file = NULL;

    if (MP_OBJ_IS_TYPE(_data, &mp_type_fileio) || MP_OBJ_IS_TYPE(_data, &mp_type_textio)) {
        mp_obj_fdfile_t *fdfile = MP_OBJ_TO_PTR(_data);

        obj->file = fdopen(fdfile->fd, "r");
    } else if (MP_OBJ_IS_STR_OR_BYTES(_data)) {
        mp_buffer_info_t buf_info;
        mp_get_buffer(_data, &buf_info, MP_BUFFER_READ);

        obj->memfile.buffer = buf_info.buf;
        obj->memfile.length = buf_info.len;
        obj->memfile.cursor = 0;
    } else {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError, "expected str/bytes/fileio type"));
    }

    _mod__img__PngLoader_setup(obj, obj->file == NULL);

    return obj;
}

STATIC void mod__img__PngLoader_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    mp_printf(print, "<_PngLoader>");
}

STATIC mp_obj_t mod__img__PngLoader___del__(mp_obj_t self_in) {
    SELF(self_in);

    free(self->rows);
    free(self->img);

    png_destroy_read_struct(&self->png, &self->png_info, NULL);

    return mp_const_none;
}

STATIC mp_obj_t mod__img__PngLoader_load_chunk(mp_obj_t self_in) {
    SELF(self_in);

    if (setjmp(png_jmpbuf(self->png))) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_RuntimeError, "couldn't read png"));
    }

    png_read_row(self->png, self->rows[png_get_current_row_number(self->png)], NULL);

    int y = PNG_ROW_FROM_PASS_ROW(png_get_current_row_number(self->png), png_get_current_pass_number(self->png));
    if (y == self->height) {
        return mp_const_false;
    }

    return mp_const_true;
}

STATIC mp_obj_t mod__img__PngLoader_finish(mp_obj_t self_in) {
    SELF(self_in);

    mp_obj_t ret = mp_obj_new_bytes(self->img, self->img_len);

    mp_obj_tuple_t *tuple = mp_obj_new_tuple(4, NULL);
    tuple->items[0] = mp_obj_new_int(self->width);
    tuple->items[1] = mp_obj_new_int(self->height);
    tuple->items[2] = mp_obj_new_int(self->channels);
    tuple->items[3] = ret;

    return tuple;
}

STATIC mp_obj_t mod__img__PngLoader_load_all(mp_obj_t self_in) {
    SELF(self_in);

    png_read_image(self->png, self->rows);

    return mod__img__PngLoader_finish(self_in);
}

STATIC mp_obj_t mod__img__PngLoader_load_in_background(mp_obj_t self_in) {
    SELF(self_in);

    s32 prio = 0;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    Thread thread = threadCreate(_mod__img__PngLoader_bg_thread, self_in, BG_THREAD_STACK_SIZE, prio - 1, -2, true);
    self->bg_thread = thread;
    self->bg_complete = false;
    self->bg_finished = false;

    return mp_const_none;
}

STATIC mp_obj_t mod__img__PngLoader_get_image(mp_obj_t self_in) {
    SELF(self_in);

    if (!self->bg_complete) {
        return mp_const_none;
    } else if (self->bg_finished) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_RuntimeError, "cannot call get_image() twice once the image has been loaded"));
    }

    self->bg_finished = true;

    return mod__img__PngLoader_finish(self_in);
}

METHOD_OBJ_N(1, __del__);
METHOD_OBJ_N(1, load_chunk);
METHOD_OBJ_N(1, finish);
METHOD_OBJ_N(1, load_all);
METHOD_OBJ_N(1, load_in_background);
METHOD_OBJ_N(1, get_image);

STATIC const mp_map_elem_t mod__img__PngLoader_locals_dict_table[] = {
        // Methods
        LOCAL_METHOD(__del__),
        LOCAL_METHOD(load_chunk),
        LOCAL_METHOD(finish),
        LOCAL_METHOD(load_all),
        LOCAL_METHOD(load_in_background),
        LOCAL_METHOD(get_image),
};
STATIC MP_DEFINE_CONST_DICT(mod__img__PngLoader_locals_dict, mod__img__PngLoader_locals_dict_table);

const mp_obj_type_t mod__img__PngLoader_type = {
        {&mp_type_type},
        .name = MP_QSTR__PngLoader,
        .print = mod__img__PngLoader_print,
        .make_new = mod__img__PngLoader_make_new,
        .locals_dict = (mp_obj_t) &mod__img__PngLoader_locals_dict,
};