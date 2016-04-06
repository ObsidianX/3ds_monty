#include <string.h>
#include <stdio.h>

#include <3ds.h>

#include "py/runtime.h"
#include "py/objstr.h"
#include "py/stream.h"

#include "../helpers.h"
#include "../../file.h"

#define SELF(src) mod_citrus_ndsp_Sound_t *self = src;

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_ndsp_Sound_##__n##_obj, mod_citrus_ndsp_Sound_##__n)

#define ATTR_METHOD(__n) \
    if(!strcmp(name, qstr_str(MP_QSTR_##__n))) { \
        dest[0] = (mp_obj_t) &mod_citrus_ndsp_Sound_##__n##_obj; \
    }

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_ndsp_Sound_##__n##_obj}

#define RIFF_MAGIC     "RIFF"
#define FORMAT_WAVE    "WAVE"
#define FMT_FORMAT_PCM 1

typedef struct {
    char magic[4];
    u32 size;
    char format[4];
} riff_header;

typedef struct {
    char id[4];
    u32 size;
    u16 format;
    u16 channels;
    u32 sample_rate;
    u32 byte_rate;
    u16 block_align;
    u16 bit_rate;
} riff_wave_fmt_chunk;

typedef struct {
    char id[4];
    u32 size;
    /* void *data; */
} riff_wave_data_chunk;

enum {
    NEW_ARG_DATA = 0,
    NEW_ARG_LOOP,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_ndsp_Sound_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 0, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_data, MP_ARG_OBJ | MP_ARG_REQUIRED, {}},
            {MP_QSTR_loop, MP_ARG_BOOL,                  {.u_bool = false}}
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mp_obj_t _data = args[NEW_ARG_DATA].u_obj;
    bool cleanup = false;
    if (MP_OBJ_IS_TYPE(_data, &mp_type_fileio)) {
        // utilize stream.readall()
        _data = mp_stream_readall_obj.fun._1(_data);
        cleanup = true;
    }

    mp_buffer_info_t buf_info;

    if (MP_OBJ_IS_STR_OR_BYTES(_data)) {
        mp_get_buffer(_data, &buf_info, MP_BUFFER_READ);
    } else {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError, "expected str/bytes/fileio type"));
    }

    riff_header *riff = (riff_header *) buf_info.buf;

    if (strncmp(RIFF_MAGIC, riff->magic, 4)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "expected RIFF file"));
    }

    if (strncmp(FORMAT_WAVE, riff->format, 4)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "expected WAVE format"));
    }

    riff_wave_fmt_chunk *fmt = (riff_wave_fmt_chunk *) (buf_info.buf + sizeof(riff_header));

    if (fmt->format != FMT_FORMAT_PCM) {
        char out[64];
        sprintf(out, "unknown WAVE format. expected PCM (1), got (%d)", fmt->format);
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, out));
    }

    riff_wave_data_chunk *wave_data = (riff_wave_data_chunk *) (buf_info.buf + sizeof(riff_header) + sizeof(riff_wave_fmt_chunk));

    mod_citrus_ndsp_Sound_t *obj = m_new_obj(mod_citrus_ndsp_Sound_t);
    obj->base.type = (mp_obj_t) &mod_citrus_ndsp_Sound_type;

    memset(&obj->wave, 0, sizeof(ndspWaveBuf));

    obj->wave.nsamples = wave_data->size / (fmt->bit_rate >> 3);
    obj->wave.status = NDSP_WBUF_FREE;
    obj->wave.looping = args[NEW_ARG_LOOP].u_bool;

    obj->data = linearAlloc(wave_data->size);
    if (!obj->data) {
        nlr_raise(mp_obj_new_exception(&mp_type_MemoryError));
    }
    void *data_start = buf_info.buf + sizeof(riff_header) + sizeof(riff_wave_fmt_chunk) + sizeof(riff_wave_data_chunk);
    memcpy(obj->data, data_start, wave_data->size);
    obj->wave.data_vaddr = obj->data;
    obj->data_size = wave_data->size;

    obj->sample_rate = fmt->sample_rate;
    if (fmt->bit_rate == 8) {
        obj->format = (fmt->channels == 1) ? NDSP_FORMAT_MONO_PCM8 : NDSP_FORMAT_STEREO_PCM8;
    } else {
        obj->format = (fmt->channels == 1) ? NDSP_FORMAT_MONO_PCM16 : NDSP_FORMAT_STEREO_PCM16;
    }

    DSP_FlushDataCache(obj->data, obj->data_size);

    if (cleanup) {
        m_del_obj(mp_obj_get_type(_data), _data);
    }

    return obj;
}

STATIC void mod_citrus_ndsp_Sound_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<Sound wave=%p>", self->wave);
}

STATIC mp_obj_t mod_citrus_ndsp_Sound___del__(mp_obj_t self_in) {
    SELF(self_in);
    linearFree(self->data);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_Sound_get_status(mp_obj_t self_in) {
    SELF(self_in);

    return mp_obj_new_int(self->wave.status);
}

METHOD_OBJ_N(1, __del__);
METHOD_OBJ_N(1, get_status);

STATIC void mod_citrus_ndsp_Sound_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    SELF(self_in);
    const char *name = qstr_str(attr);
    bool load = dest[0] == MP_OBJ_NULL;

    // Methods
    if (load) {
        ATTR_METHOD(get_status)

        if (dest[0] != MP_OBJ_NULL) {
            dest[1] = self_in;
            return;
        }
    }

    if (!strcmp(name, qstr_str(MP_QSTR_loop))) {
        if (load) {
            dest[0] = mp_obj_new_bool(self->wave.looping);
        } else {
            self->wave.looping = mp_obj_is_true(dest[1]);
            dest[0] = NULL;
        }
    }
}

STATIC const mp_map_elem_t mod_citrus_ndsp_Sound_locals_dict_table[] = {
        // Methods
        LOCAL_METHOD(__del__),
        LOCAL_METHOD(get_status),
};
STATIC MP_DEFINE_CONST_DICT(mod_citrus_ndsp_Sound_locals_dict, mod_citrus_ndsp_Sound_locals_dict_table);

const mp_obj_type_t mod_citrus_ndsp_Sound_type = {
        {&mp_type_type},
        .name = MP_QSTR_Sound,
        .print = mod_citrus_ndsp_Sound_print,
        .make_new = mod_citrus_ndsp_Sound_make_new,
        .locals_dict = (mp_obj_t) &mod_citrus_ndsp_Sound_locals_dict,
        .attr = (mp_obj_t) &mod_citrus_ndsp_Sound_attr,
};