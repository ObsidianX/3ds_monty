#include <string.h>
#include <malloc.h>

#include <3ds.h>

#include "py/runtime.h"

#define SELF(src) mod_citrus_csnd_Sound_t *self = src

#define LOAD_STORE_ATTR(_load, _name, _type) \
    if (_load) { \
        dest[0] = mp_obj_new_##_type(self->_name); \
    } else { \
        self->_name = mp_obj_get_##_type(dest[0]); \
        dest[0] = MP_OBJ_NULL; \
    }

const mp_obj_type_t mod_citrus_csnd_Sound_type;
STATIC const mp_obj_fun_builtin_t mod_citrus_csnd_Sound___del___obj;

typedef struct {
    mp_obj_base_t base;

    int channel;
    u32 flags;
    u32 sample_rate;
    float volume;
    float pan;
    void *data;
    u32 size;
} mod_citrus_csnd_Sound_t;

STATIC void _mod_citrus_csnd_Sound_data_from_string(mp_obj_t obj, mp_obj_t data) {
    SELF(obj);

    if (MP_OBJ_IS_STR(data)) {
        mp_buffer_info_t bufferinfo;
        mp_get_buffer_raise(data, &bufferinfo, MP_BUFFER_READ);

        self->data = linearAlloc(bufferinfo.len);
        if (!self->data) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_MemoryError, "not enough memory to allocate sound buffer"));
        }
        memcpy(bufferinfo.buf, self->data, bufferinfo.len);
        self->size = bufferinfo.len;
    } else {
        self->data = NULL;
        self->size = 0;
    }
}

enum {
    NEW_ARG_CHANNEL = 0,
    NEW_ARG_FLAGS,
    NEW_ARG_SAMPLE_RATE,
    NEW_ARG_VOLUME,
    NEW_ARG_PAN,
    NEW_ARG_DATA,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_csnd_Sound_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 1, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_channel,     MP_ARG_INT, {.u_int = -1}},
            {MP_QSTR_flags,       MP_ARG_INT, {.u_int = 0}},
            {MP_QSTR_sample_rate, MP_ARG_INT, {.u_int = 0}},
            {MP_QSTR_volume,      MP_ARG_OBJ, {.u_obj = mp_const_none}},
            {MP_QSTR_pan,         MP_ARG_OBJ, {.u_obj = mp_const_none}},
            {MP_QSTR_data,        MP_ARG_OBJ, {.u_obj = mp_const_none}},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    float vol = 1.0f;
    float pan = 0.0f;

    mp_obj_t vol_obj = args[NEW_ARG_VOLUME].u_obj;
    mp_obj_t pan_obj = args[NEW_ARG_PAN].u_obj;
    mp_obj_t data_obj = args[NEW_ARG_DATA].u_obj;

    if (mp_obj_is_float(vol_obj)) {
        vol = mp_obj_get_float(vol_obj);
    }
    if (mp_obj_is_float(pan_obj)) {
        pan = mp_obj_get_float(pan_obj);
    }

    mod_citrus_csnd_Sound_t *obj = m_new_obj(mod_citrus_csnd_Sound_t);
    obj->base.type = (mp_obj_t) &mod_citrus_csnd_Sound_type;

    obj->channel = args[NEW_ARG_CHANNEL].u_int;
    obj->flags = args[NEW_ARG_FLAGS].u_int;
    obj->sample_rate = args[NEW_ARG_SAMPLE_RATE].u_int;
    obj->volume = vol;
    obj->pan = pan;

    _mod_citrus_csnd_Sound_data_from_string(obj, data_obj);

    return obj;
}

// implements load, store and delete attribute
//
// dest[0] = MP_OBJ_NULL means load
//  return: for fail, do nothing
//          for attr, dest[0] = value
//          for method, dest[0] = method, dest[1] = self
//
// dest[0,1] = {MP_OBJ_SENTINEL, MP_OBJ_NULL} means delete
// dest[0,1] = {MP_OBJ_SENTINEL, object} means store
//  return: for fail, do nothing
//          for success set dest[0] = MP_OBJ_NULL
STATIC void mod_citrus_csnd_Sound_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    const char *name = qstr_str(attr);
    if (dest[0] == MP_OBJ_SENTINEL && dest[1] == MP_OBJ_NULL) {
        // deleting not supported
        return;
    }
    bool load = dest[0] == MP_OBJ_NULL;

    // Methods
    if (load) {
        if (!strcmp(name, qstr_str(MP_QSTR___del__))) {
            dest[0] = (mp_obj_t) &mod_citrus_csnd_Sound___del___obj;
        }
        if (dest[0] != MP_OBJ_NULL) {
            dest[1] = self_in;
            return;
        }
    }

    SELF(self_in);

    // Attributes
    // dest[0] = value (load)
    // dest[0] = MP_OBJ_NULL (store)
    if (!strcmp(name, qstr_str(MP_QSTR_channel))) {
        LOAD_STORE_ATTR(load, channel, int);
    } else if (!strcmp(name, qstr_str(MP_QSTR_flags))) {
        LOAD_STORE_ATTR(load, flags, int);
    } else if (!strcmp(name, qstr_str(MP_QSTR_sample_rate))) {
        LOAD_STORE_ATTR(load, sample_rate, int);
    } else if (!strcmp(name, qstr_str(MP_QSTR_volume))) {
        LOAD_STORE_ATTR(load, volume, float);
    } else if (!strcmp(name, qstr_str(MP_QSTR_pan))) {
        LOAD_STORE_ATTR(load, pan, float);
    } else if (!strcmp(name, qstr_str(MP_QSTR_data))) {
        if (!load) {
            _mod_citrus_csnd_Sound_data_from_string(self_in, dest[0]);
        }
    }
}

STATIC void mod_citrus_csnd_Sound_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    //SELF(self_in);

    mp_printf(print, "<Sound >");
}

STATIC mp_obj_t mod_citrus_csnd_Sound___del__(mp_obj_t self_in) {
    SELF(self_in);

    if (self->data != NULL) {
        free(self->data);
    }

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_csnd_Sound_play(void) {
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_csnd_Sound___del___obj, mod_citrus_csnd_Sound___del__);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_csnd_Sound_play_obj, mod_citrus_csnd_Sound_play);

STATIC const mp_map_elem_t mod_citrus_csnd_Sound_locals_dict_table[] = {
        // Methods
        {MP_OBJ_NEW_QSTR(MP_QSTR___del__), (mp_obj_t) &mod_citrus_csnd_Sound___del___obj},
        {MP_OBJ_NEW_QSTR(MP_QSTR_play),    (mp_obj_t) &mod_citrus_csnd_Sound_play_obj},
};
STATIC MP_DEFINE_CONST_DICT(mod_citrus_csnd_Sound_locals_dict, mod_citrus_csnd_Sound_locals_dict_table);

const mp_obj_type_t mod_citrus_csnd_Sound_type = {
        {&mp_type_type},
        .name = MP_QSTR_Sound,
        .print = mod_citrus_csnd_Sound_print,
        .make_new = mod_citrus_csnd_Sound_make_new,
        .locals_dict = (mp_obj_t) &mod_citrus_csnd_Sound_locals_dict,
        .attr = (mp_obj_t) &mod_citrus_csnd_Sound_attr,
};