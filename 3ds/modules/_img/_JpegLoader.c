#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <3ds.h>
#include <jpeglib.h>

#include "py/runtime.h"
#include "py/objstr.h"
#include "py/stream.h"

#include "../file.h"
#include "common.h"

#define SELF(src) mod__img__JpegLoader_t *self = src

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod__img__JpegLoader_##__n##_obj, mod__img__JpegLoader_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod__img__JpegLoader_##__n##_obj}

#define BG_THREAD_STACK_SIZE (2 * 1024)

const mp_obj_type_t mod__img__JpegLoader_type;
STATIC mp_obj_t mod__img__JpegLoader_load_all(mp_obj_t self_in);

struct jpeg_err_mgr {
    struct jpeg_error_mgr pub;

    jmp_buf setjmp_buffer;
};

typedef struct {
    mp_obj_base_t base;

    FILE *file;
    mem_file memfile;

    struct jpeg_decompress_struct cinfo;
    struct jpeg_err_mgr err;

    int row_stride;

    size_t img_len;
    unsigned char *img;

    JSAMPARRAY buffer;

    Thread bg_thread;
    bool bg_complete;
    bool bg_finished;
} mod__img__JpegLoader_t;

STATIC void _mod_img_jpeg_raise(j_common_ptr cinfo) {
    struct jpeg_err_mgr *err = (struct jpeg_err_mgr *) cinfo->err;
    longjmp(err->setjmp_buffer, 1);
}

STATIC void _mod__img__JpegLoader_bg_thread(void *self_in) {
    SELF(self_in);

    mod__img__JpegLoader_load_all(self_in);

    self->bg_complete = true;

    threadExit(0);
}

STATIC void _mod__img__JpegLoader_setup(mp_obj_t self_in, bool from_memory) {
    SELF(self_in);

    self->buffer = (*self->cinfo.mem->alloc_sarray)((j_common_ptr) &self->cinfo, JPOOL_IMAGE, self->row_stride, 1);

    self->cinfo.err = jpeg_std_error(&self->err.pub);
    self->err.pub.error_exit = _mod_img_jpeg_raise;

    if (setjmp(self->err.setjmp_buffer)) {
        jpeg_destroy_decompress(&self->cinfo);
        nlr_raise(mp_obj_new_exception_msg(&mp_type_RuntimeError, "Couldn't decompress jpeg"));
    }

    jpeg_create_decompress(&self->cinfo);
    if (from_memory) {
        jpeg_mem_src(&self->cinfo, self->memfile.buffer, self->memfile.length);
    } else {
        jpeg_stdio_src(&self->cinfo, self->file);
    }
    jpeg_read_header(&self->cinfo, true);
    jpeg_start_decompress(&self->cinfo);

    self->row_stride = self->cinfo.output_width * self->cinfo.output_components;

    self->img_len = self->row_stride * self->cinfo.output_height;
    self->img = malloc(self->img_len);
}

enum {
    NEW_ARG_DATA = 0,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod__img__JpegLoader_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 1, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_data, MP_ARG_OBJ | MP_ARG_REQUIRED},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod__img__JpegLoader_t *obj = m_new_obj(mod__img__JpegLoader_t);
    obj->base.type = (mp_obj_t) &mod__img__JpegLoader_type;

    mp_obj_t _data = args[NEW_ARG_DATA].u_obj;

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

    _mod__img__JpegLoader_setup(obj, obj->file == NULL);

    return obj;
}

STATIC void mod__img__JpegLoader_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    mp_printf(print, "<_JpegLoader>");
}

STATIC mp_obj_t mod__img__JpegLoader___del__(mp_obj_t self_in) {
    SELF(self_in);

    jpeg_finish_decompress(&self->cinfo);
    jpeg_destroy_decompress(&self->cinfo);

    free(self->img);

    return mp_const_none;
}

STATIC mp_obj_t mod__img__JpegLoader_load_chunk(mp_obj_t self_in) {
    SELF(self_in);

    jpeg_read_scanlines(&self->cinfo, self->buffer, 1);
    memcpy(&self->img[self->row_stride * self->cinfo.output_scanline], self->buffer, self->row_stride);

    if (self->cinfo.output_scanline >= self->cinfo.output_height) {
        return mp_const_false;
    }

    return mp_const_true;
}

STATIC mp_obj_t mod__img__JpegLoader_finish(mp_obj_t self_in) {
    SELF(self_in);

    mp_obj_t ret = mp_obj_new_bytes(self->img, self->img_len);

    mp_obj_tuple_t *tuple = mp_obj_new_tuple(4, NULL);
    tuple->items[0] = mp_obj_new_int(self->cinfo.image_width);
    tuple->items[1] = mp_obj_new_int(self->cinfo.image_height);
    tuple->items[2] = mp_obj_new_int(self->cinfo.output_components);
    tuple->items[3] = ret;

    return ret;
}

STATIC mp_obj_t mod__img__JpegLoader_load_all(mp_obj_t self_in) {
    SELF(self_in);

    while (self->cinfo.output_scanline < self->cinfo.output_height) {
        jpeg_read_scanlines(&self->cinfo, self->buffer, 1);
        memcpy(&self->img[self->row_stride * self->cinfo.output_scanline], self->buffer, self->row_stride);
    }

    return mod__img__JpegLoader_finish(self_in);
}

STATIC mp_obj_t mod__img__JpegLoader_load_in_background(mp_obj_t self_in) {
    SELF(self_in);

    s32 prio = 0;
    svcGetThreadPriority(&prio, CUR_THREAD_HANDLE);
    Thread thread = threadCreate(_mod__img__JpegLoader_bg_thread, self_in, BG_THREAD_STACK_SIZE, prio - 1, -2, true);
    self->bg_thread = thread;
    self->bg_complete = false;
    self->bg_finished = false;

    return mp_const_none;
}

STATIC mp_obj_t mod__img__JpegLoader_get_image(mp_obj_t self_in) {
    SELF(self_in);

    if (!self->bg_complete) {
        return mp_const_none;
    } else if (self->bg_finished) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_RuntimeError, "cannot call get_image() twice once the image has been loaded"));
    }

    self->bg_finished = true;

    return mod__img__JpegLoader_finish(self_in);
}

METHOD_OBJ_N(1, __del__);
METHOD_OBJ_N(1, load_chunk);
METHOD_OBJ_N(1, finish);
METHOD_OBJ_N(1, load_all);
METHOD_OBJ_N(1, load_in_background);
METHOD_OBJ_N(1, get_image);

STATIC const mp_map_elem_t mod__img__JpegLoader_locals_dict_table[] = {
        // Methods
        LOCAL_METHOD(__del__),
        LOCAL_METHOD(load_chunk),
        LOCAL_METHOD(finish),
        LOCAL_METHOD(load_all),
        LOCAL_METHOD(load_in_background),
        LOCAL_METHOD(get_image),
};
STATIC MP_DEFINE_CONST_DICT(mod__img__JpegLoader_locals_dict, mod__img__JpegLoader_locals_dict_table);

const mp_obj_type_t mod__img__JpegLoader_type = {
        {&mp_type_type},
        .name = MP_QSTR__JpegLoader,
        .print = mod__img__JpegLoader_print,
        .make_new = mod__img__JpegLoader_make_new,
        .locals_dict = (mp_obj_t) &mod__img__JpegLoader_locals_dict,
};