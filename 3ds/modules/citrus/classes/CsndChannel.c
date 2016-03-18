#include <string.h>

#include <3ds.h>

#include "py/runtime.h"

#define SELF(src) mod_citrus_csnd_Channel_t *self = src

const mp_obj_type_t mod_citrus_csnd_Channel_type;
STATIC const mp_obj_fun_builtin_t mod_citrus_csnd_Channel___del___obj;

typedef struct {
    mp_obj_base_t base;
} mod_citrus_csnd_Channel_t;

STATIC mp_obj_t mod_citrus_csnd_Channel_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    return mp_const_none;
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
STATIC void mod_citrus_csnd_Channel_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    const char *name = qstr_str(attr);
    if (dest[0] == MP_OBJ_SENTINEL && dest[1] == MP_OBJ_NULL) {
        // deleting not supported
        return;
    }
    bool load = dest[0] == MP_OBJ_NULL;

    // Methods
    if (load) {
        if (!strcmp(name, qstr_str(MP_QSTR___del__))) {
            dest[0] = (mp_obj_t) &mod_citrus_csnd_Channel___del___obj;
        }
        if (dest[0] != MP_OBJ_NULL) {
            dest[1] = self_in;
            return;
        }
    }

    // Attributes
    // dest[0] = value (load)
    // dest[0] = MP_OBJ_NULL (store)
}

STATIC void mod_citrus_csnd_Channel_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    //SELF(self_in);

    mp_printf(print, "<Channel >");
}

STATIC mp_obj_t mod_citrus_csnd_Channel___del__(mp_obj_t self_in) {
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_csnd_Channel___del___obj, mod_citrus_csnd_Channel___del__);

STATIC const mp_map_elem_t mod_citrus_csnd_Channel_locals_dict_table[] = {
        // Methods
        {MP_OBJ_NEW_QSTR(MP_QSTR___del__), (mp_obj_t) &mod_citrus_csnd_Channel___del___obj},
};
STATIC MP_DEFINE_CONST_DICT(mod_citrus_csnd_Channel_locals_dict, mod_citrus_csnd_Channel_locals_dict_table);

const mp_obj_type_t mod_citrus_csnd_Channel_type = {
        {&mp_type_type},
        .name = MP_QSTR_Channel,
        .print = mod_citrus_csnd_Channel_print,
        .make_new = mod_citrus_csnd_Channel_make_new,
        .locals_dict = (mp_obj_t) &mod_citrus_csnd_Channel_locals_dict,
        .attr = (mp_obj_t) &mod_citrus_csnd_Channel_attr,
};