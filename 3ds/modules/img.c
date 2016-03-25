#include <unistd.h>
#include <string.h>
#include <malloc.h>

#include <png.h>
#include <jpeglib.h>

#include "py/runtime.h"
#include "py/stream.h"
#include "py/objstr.h"

#include "file.h"

typedef struct {
    unsigned char *buffer;
    size_t length;
    size_t cursor;
} mem_file;

struct jpeg_err_mgr {
    struct jpeg_error_mgr pub;

    jmp_buf setjmp_buffer;
};

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

STATIC void _mod_img_jpeg_raise(j_common_ptr cinfo) {
    struct jpeg_err_mgr *err = (struct jpeg_err_mgr *) cinfo->err;
    longjmp(err->setjmp_buffer, 1);
}

STATIC mp_obj_t mod_img_load_png(mp_obj_t data) {
    mp_obj_t _data = data;
    mem_file file;
    bool cleanup = false;

    if (MP_OBJ_IS_TYPE(_data, &mp_type_fileio)) {
        // utilize stream.readall()
        _data = mp_stream_readall_obj.fun._1(_data);
        cleanup = true;
    }

    if (MP_OBJ_IS_STR_OR_BYTES(_data)) {
        mp_buffer_info_t buf_info;
        mp_get_buffer(_data, &buf_info, MP_BUFFER_READ);

        file.buffer = buf_info.buf;
        file.length = buf_info.len;
        file.cursor = 0;
    } else {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError, "expected str/bytes/fileio type"));
    }

    if (png_sig_cmp(file.buffer, 0, 8)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "PNG data not found"));
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_RuntimeError, "couldn't create png_struct"));
    }

    png_infop png_info = png_create_info_struct(png);
    if (!png_info) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_RuntimeError, "couldn't create png_info"));
    }

    png_set_read_fn(png, &file, &_mod_img_mem_file_read);

    png_set_sig_bytes(png, 0);

    png_read_info(png, png_info);

    png_uint_32 width = png_get_image_width(png, png_info);
    png_uint_32 height = png_get_image_height(png, png_info);
    png_byte channels = png_get_channels(png, png_info);

    size_t row_len = png_get_rowbytes(png, png_info);
    size_t img_len = sizeof(png_byte) * height * row_len;
    png_bytep img = malloc(img_len);

    png_bytepp rows = malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        rows[y] = &img[y * row_len];
    }

    if (setjmp(png_jmpbuf(png))) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_RuntimeError, "couldn't read png"));
    }

    png_read_image(png, rows);

    mp_obj_t ret = mp_obj_new_bytes(img, img_len);

    mp_buffer_info_t buf_inf;
    mp_get_buffer(ret, &buf_inf, MP_BUFFER_READ);

    mp_obj_tuple_t *tuple = mp_obj_new_tuple(4, NULL);
    tuple->items[0] = mp_obj_new_int(width);
    tuple->items[1] = mp_obj_new_int(height);
    tuple->items[2] = mp_obj_new_int(channels);
    tuple->items[3] = ret;

    free(rows);
    free(img);

    if (cleanup) {
        m_del_obj(mp_obj_get_type(_data), _data);
    }

    return tuple;
}

STATIC mp_obj_t mod_img_load_jpeg(mp_obj_t data) {
    mp_obj_t _data = data;
    mem_file file;
    bool cleanup = false;

    if (MP_OBJ_IS_TYPE(_data, &mp_type_fileio)) {
        // utilize stream.readall()
        _data = mp_stream_readall_obj.fun._1(_data);
        cleanup = true;
    }

    if (MP_OBJ_IS_STR_OR_BYTES(_data)) {
        mp_buffer_info_t buf_info;
        mp_get_buffer(_data, &buf_info, MP_BUFFER_READ);

        file.buffer = buf_info.buf;
        file.length = buf_info.len;
        file.cursor = 0;
    } else {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError, "expected str/bytes/fileio type"));
    }

    struct jpeg_decompress_struct cinfo;
    struct jpeg_err_mgr err;

    JSAMPARRAY buffer;
    int row_stride;

    cinfo.err = jpeg_std_error(&err.pub);
    err.pub.error_exit = _mod_img_jpeg_raise;

    if (setjmp(err.setjmp_buffer)) {
        jpeg_destroy_decompress(&cinfo);
        nlr_raise(mp_obj_new_exception_msg(&mp_type_RuntimeError, "Couldn't decompress jpeg"));
    }

    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, file.buffer, file.length);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    size_t img_len = row_stride * cinfo.output_height;
    unsigned char *img = malloc(img_len);

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(&img[row_stride * cinfo.output_scanline], buffer, row_stride);
    }

    mp_obj_t ret = mp_obj_new_bytes(img, img_len);

    mp_buffer_info_t buf_inf;
    mp_get_buffer(ret, &buf_inf, MP_BUFFER_READ);

    mp_obj_tuple_t *tuple = mp_obj_new_tuple(4, NULL);
    tuple->items[0] = mp_obj_new_int(cinfo.image_width);
    tuple->items[1] = mp_obj_new_int(cinfo.image_height);
    tuple->items[2] = mp_obj_new_int(cinfo.output_components);
    tuple->items[3] = ret;

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(img);

    if (cleanup) {
        m_del_obj(mp_obj_get_type(_data), _data);
    }

    return ret;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_img_load_png_obj, mod_img_load_png);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_img_load_jpeg_obj, mod_img_load_jpeg);

STATIC const mp_rom_map_elem_t mp_module_img_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR_load_png),  MP_ROM_PTR(&mod_img_load_png_obj)},
        {MP_ROM_QSTR(MP_QSTR_load_jpeg), MP_ROM_PTR(&mod_img_load_jpeg_obj)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_img_globals, mp_module_img_globals_table);

const mp_obj_module_t mp_module_img = {
        .base = {&mp_type_module},
        .name = MP_QSTR_img,
        .globals = (mp_obj_dict_t *) &mp_module_img_globals,
};